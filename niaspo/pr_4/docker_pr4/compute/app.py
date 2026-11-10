#!/usr/bin/env python3
"""
Контейнер №2 — «вычислитель».

Слушает localhost:8082 (порт задаётся переменной окружения PORT).
Принимает POST /process с JSON {"text": "..."} от контейнера №1,
обрабатывает данные и возвращает результат контейнеру №1.
С пользователем напрямую не общается.
"""

import hashlib
import os
import socket
import time
from datetime import datetime, timezone

from flask import Flask, jsonify, request

SERVICE_NAME = os.environ.get("SERVICE_NAME", "Контейнер №2")
PORT = int(os.environ.get("PORT", "8082"))
HOST = os.environ.get("HOST", "0.0.0.0")

STARTED_AT = time.time()
app = Flask(__name__)
# JSON с читаемой кириллицей вместо \uXXXX — удобнее смотреть в curl
app.json.ensure_ascii = False
app.json.sort_keys = False


def utc_now() -> str:
    return datetime.now(timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")


@app.get("/")
def info():
    """Короткая справка о сервисе (для отладки через браузер)."""
    return jsonify(
        service=SERVICE_NAME,
        role="обработка запроса, поступившего от контейнера №1",
        hostname=socket.gethostname(),
        listen=f"{HOST}:{PORT}",
        endpoints=["GET /", "GET /health", "POST /process"],
    )


@app.get("/health")
def health():
    """Проверка живости: использует healthcheck в docker-compose.yml."""
    return jsonify(status="ok", service=SERVICE_NAME, uptime_sec=round(time.time() - STARTED_AT, 1))


@app.post("/process")
def process():
    """
    Основная логика контейнера №2.
    Вход:  {"text": "строка для обработки"}  (+ необязательный request_id в заголовке)
    Выход: результат обработки — статистика текста и его SHA-256.
    """
    started = time.perf_counter()
    request_id = request.headers.get("X-Request-Id", "-")

    payload = request.get_json(silent=True)
    if not isinstance(payload, dict):
        return jsonify(error="ожидается JSON-объект вида {\"text\": \"...\"}"), 400

    text = payload.get("text")
    if not isinstance(text, str) or not text.strip():
        return jsonify(error="поле 'text' обязательно и не может быть пустым"), 400
    if len(text) > 10000:
        return jsonify(error="поле 'text' длиннее 10000 символов"), 400

    words = text.split()

    result = {
        "request_id": request_id,
        "service": SERVICE_NAME,
        "hostname": socket.gethostname(),
        "processed_at": utc_now(),
        "input": {
            "chars": len(text),
            "preview": text[:60] + ("…" if len(text) > 60 else ""),
        },
        "result": {
            "chars": len(text),
            "chars_no_spaces": len(text.replace(" ", "")),
            "words": len(words),
            "unique_words": len({w.lower().strip(".,!?;:") for w in words}),
            "longest_word": max(words, key=len, default=""),
            "sha256": hashlib.sha256(text.encode("utf-8")).hexdigest(),
        },
        "processing_ms": round((time.perf_counter() - started) * 1000, 3),
    }

    app.logger.info("[%s] обработан запрос: %d символов, %d слов", request_id, len(text), len(words))
    return jsonify(result), 200


if __name__ == "__main__":
    app.run(host=HOST, port=PORT)
