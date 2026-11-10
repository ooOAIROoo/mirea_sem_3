#!/usr/bin/env python3
"""
Контейнер №1 — «шлюз».

Слушает localhost:8081 (порт задаётся переменной окружения PORT).
Принимает запрос пользователя, сам обращается к контейнеру №2 по адресу
localhost:8082 и возвращает пользователю результат, полученный от него.
"""

import os
import socket
import time
import uuid
from datetime import datetime, timezone

import requests
from flask import Flask, jsonify, render_template, request

SERVICE_NAME = os.environ.get("SERVICE_NAME", "Контейнер №1")
PORT = int(os.environ.get("PORT", "8081"))
HOST = os.environ.get("HOST", "0.0.0.0")

# Адрес контейнера №2. В этой работе он равен localhost, потому что оба
# контейнера запущены в сети хоста (network_mode: host) и видят общий
# интерфейс loopback. Порт отличается — 8082.
NEXT_URL = os.environ.get("NEXT_URL", "http://localhost:8082")
UPSTREAM_TIMEOUT = float(os.environ.get("UPSTREAM_TIMEOUT", "5"))

STARTED_AT = time.time()
app = Flask(__name__)
# JSON с читаемой кириллицей вместо \uXXXX — удобнее смотреть в curl
app.json.ensure_ascii = False
app.json.sort_keys = False


def utc_now() -> str:
    return datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")


def call_next(text: str, request_id: str) -> tuple[dict | None, dict]:
    """
    Запрос к контейнеру №2. Возвращает (ответ, служебные данные о вызове).
    Ошибки сети не выбрасываются наружу, а превращаются в описание —
    так пользователь видит, на каком шаге цепочка прервалась.
    """
    started = time.perf_counter()
    meta = {"url": f"{NEXT_URL}/process", "ok": False}
    try:
        response = requests.post(
            f"{NEXT_URL}/process",
            json={"text": text},
            headers={"X-Request-Id": request_id},
            timeout=UPSTREAM_TIMEOUT,
        )
        meta["status"] = response.status_code
        meta["elapsed_ms"] = round((time.perf_counter() - started) * 1000, 3)
        if response.status_code == 200:
            meta["ok"] = True
            return response.json(), meta
        meta["error"] = response.text[:200]
        return None, meta
    except requests.RequestException as exc:
        meta["elapsed_ms"] = round((time.perf_counter() - started) * 1000, 3)
        meta["error"] = f"{type(exc).__name__}: {exc}"
        return None, meta


@app.get("/")
def index():
    """Страница для пользователя: поле ввода и кнопка «Отправить запрос»."""
    return render_template("index.html", service=SERVICE_NAME, port=PORT, next_url=NEXT_URL)


@app.get("/health")
def health():
    return jsonify(status="ok", service=SERVICE_NAME, uptime_sec=round(time.time() - STARTED_AT, 1))


@app.get("/chain")
def chain():
    """
    Диагностика цепочки: контейнер №1 проверяет, доступен ли контейнер №2.
    Используется для healthcheck.
    """
    try:
        response = requests.get(f"{NEXT_URL}/health", timeout=2)
        upstream = {"ok": response.status_code == 200, "body": response.json()}
    except requests.RequestException as exc:
        upstream = {"ok": False, "error": f"{type(exc).__name__}: {exc}"}
        return jsonify(service=SERVICE_NAME, upstream=upstream), 503
    return jsonify(service=SERVICE_NAME, upstream=upstream)


@app.post("/api/process")
def api_process():
    """
    Основной эндпоинт работы.
    Пользователь -> контейнер №1 (/api/process) -> контейнер №2 (/process)
             -> контейнер №1 -> пользователь.
    """
    request_id = uuid.uuid4().hex[:12]
    started = time.perf_counter()

    if request.is_json:
        payload = request.get_json(silent=True) or {}
        text = payload.get("text")
    else:
        text = request.form.get("text")

    if not isinstance(text, str) or not text.strip():
        return jsonify(error="поле 'text' обязательно", request_id=request_id), 400

    result, upstream_meta = call_next(text, request_id)
    total_ms = round((time.perf_counter() - started) * 1000, 3)

    app.logger.info(
        "[%s] пользователь -> %s -> %s: %s, %.1f мс",
        request_id, SERVICE_NAME, NEXT_URL, "ok" if upstream_meta["ok"] else "ошибка", total_ms,
    )

    if result is None:
        return jsonify(
            error="контейнер №2 не вернул результат",
            request_id=request_id,
            service=SERVICE_NAME,
            upstream=upstream_meta,
        ), 502

    return jsonify(
        request_id=request_id,
        gateway={
            "service": SERVICE_NAME,
            "hostname": socket.gethostname(),
            "listen": f"{HOST}:{PORT}",
            "received_at": utc_now(),
        },
        upstream=upstream_meta,
        answer=result,
        total_ms=total_ms,
    ), 200


if __name__ == "__main__":
    app.run(host=HOST, port=PORT)
