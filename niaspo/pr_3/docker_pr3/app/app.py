#!/usr/bin/env python3
"""Собственное приложение для ПР №3 — «Журнал записей».

Небольшой REST-сервис на Flask, который пишет и читает данные в MySQL.
Запросы приходят либо напрямую на порт 5000, либо через Apache
(страница сайта обращается к относительному пути /api/).

Эндпоинты:
    GET    /            — сведения о сервисе
    GET    /health      — проверка доступности БД (используется healthcheck-ом)
    GET    /notes       — список записей (?limit=50)
    POST   /notes       — создать запись {"title": "...", "body": "..."}
    DELETE /notes/<id>  — удалить запись
"""

import os
import time

import pymysql
from flask import Flask, jsonify, request

app = Flask(__name__)

# Ошибки, при которых приложение обязано ответить 503, а не упасть:
# - MySQLError — сеть, неверный пароль, недоступная БД;
# - RuntimeError — PyMySQL поднимает его, если для caching_sha2_password
#   не установлен пакет cryptography (см. requirements.txt).
DB_ERRORS = (pymysql.err.MySQLError, RuntimeError)

DB_CONFIG = {
    "host": os.getenv("DB_HOST", "db"),
    "port": int(os.getenv("DB_PORT", "3306")),
    "user": os.getenv("DB_USER", "notes_app"),
    "password": os.getenv("DB_PASSWORD", ""),
    "database": os.getenv("DB_NAME", "notes"),
    "charset": "utf8mb4",
    "cursorclass": pymysql.cursors.DictCursor,
    "connect_timeout": 5,
    "autocommit": True,
}

# Страховка на случай, если контейнер db стартовал без init-скрипта:
# таблица будет создана самим приложением.
SCHEMA = """
CREATE TABLE IF NOT EXISTS notes (
    id         INT AUTO_INCREMENT PRIMARY KEY,
    title      VARCHAR(200) NOT NULL,
    body       TEXT         NOT NULL,
    created_at TIMESTAMP    NOT NULL DEFAULT CURRENT_TIMESTAMP,
    INDEX idx_notes_created (created_at)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4
  COLLATE = utf8mb4_unicode_ci
"""


def run_query(sql, params=(), fetch=False):
    """Выполнить один запрос и вернуть строки (fetch=True) или lastrowid."""
    conn = pymysql.connect(**DB_CONFIG)
    try:
        cur = conn.cursor()
        try:
            cur.execute(sql, params)
            return cur.fetchall() if fetch else cur.lastrowid
        finally:
            cur.close()
    finally:
        conn.close()


def serialize_note(note):
    """Привести запись к виду, пригодному для JSON (created_at — строка)."""
    value = note.get("created_at")
    if value is not None and not isinstance(value, str):
        note["created_at"] = value.strftime("%Y-%m-%d %H:%M:%S")
    return note


def wait_for_db(attempts=10, delay=2.0):
    """Подождать MySQL: после docker compose up база поднимается не мгновенно."""
    last_error = None
    for attempt in range(1, attempts + 1):
        try:
            run_query("SELECT 1")
            return True
        except DB_ERRORS as exc:  # OperationalError и наследники
            last_error = exc
            app.logger.warning("MySQL недоступен (попытка %s/%s): %s", attempt, attempts, exc)
            time.sleep(delay)
    app.logger.error("Не удалось подключиться к MySQL: %s", last_error)
    return False


# При старте gunicorn модуль импортируется один раз — готовим схему.
try:
    if wait_for_db():
        run_query(SCHEMA)
except Exception as exc:  # noqa: BLE001 — сбой БД не должен убивать воркер
    app.logger.error("Инициализация схемы не удалась: %s", exc)


@app.get("/")
def index():
    return jsonify(
        service="pr3-app",
        description="Журнал записей: Flask + MySQL в docker compose",
        endpoints=["/health", "/notes", "POST /notes", "DELETE /notes/<id>"],
    )


@app.get("/health")
def health():
    try:
        run_query("SELECT 1")
        return jsonify(status="ok", database="up"), 200
    except DB_ERRORS as exc:
        return jsonify(status="error", database="down", detail=str(exc)), 503


@app.get("/notes")
def list_notes():
    try:
        limit = min(max(int(request.args.get("limit", 50)), 1), 200)
    except ValueError:
        return jsonify(error="параметр limit должен быть числом"), 400

    try:
        notes = run_query(
            "SELECT id, title, body, created_at FROM notes ORDER BY id DESC LIMIT %s",
            (limit,),
            fetch=True,
        )
    except DB_ERRORS as exc:
        return jsonify(error="ошибка БД", detail=str(exc)), 503

    return jsonify(count=len(notes), notes=[serialize_note(n) for n in notes])


@app.post("/notes")
def create_note():
    data = request.get_json(silent=True) or request.form
    title = (data.get("title") or "").strip()
    body = (data.get("body") or "").strip()

    if not title or not body:
        return jsonify(error="поля title и body обязательны"), 400
    if len(title) > 200:
        return jsonify(error="title не должен превышать 200 символов"), 400
    if len(body) > 10000:
        return jsonify(error="body не должен превышать 10000 символов"), 400

    try:
        new_id = run_query(
            "INSERT INTO notes (title, body) VALUES (%s, %s)", (title, body)
        )
        rows = run_query(
            "SELECT id, title, body, created_at FROM notes WHERE id = %s",
            (new_id,),
            fetch=True,
        )
    except DB_ERRORS as exc:
        return jsonify(error="ошибка БД", detail=str(exc)), 503

    note = rows[0] if rows else {"id": new_id, "title": title, "body": body}
    return jsonify(note=serialize_note(note)), 201


@app.delete("/notes/<int:note_id>")
def delete_note(note_id):
    try:
        run_query("DELETE FROM notes WHERE id = %s", (note_id,))
    except DB_ERRORS as exc:
        return jsonify(error="ошибка БД", detail=str(exc)), 503
    return jsonify(deleted=note_id), 200


if __name__ == "__main__":
    # Для локального запуска без gunicorn: python app.py
    app.run(host="0.0.0.0", port=int(os.getenv("APP_INTERNAL_PORT", "5000")))
