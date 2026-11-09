-- Инициализация базы данных приложения.
-- Скрипт выполняется автоматически при первом старте контейнера db
-- (каталог /docker-entrypoint-initdb.d). Повторно не выполняется:
-- признак инициализации — содержимое тома db_data.

CREATE TABLE IF NOT EXISTS notes (
    id         INT AUTO_INCREMENT PRIMARY KEY,
    title      VARCHAR(200) NOT NULL,
    body       TEXT         NOT NULL,
    created_at TIMESTAMP    NOT NULL DEFAULT CURRENT_TIMESTAMP,
    INDEX idx_notes_created (created_at)
) ENGINE = InnoDB
  DEFAULT CHARSET = utf8mb4
  COLLATE = utf8mb4_unicode_ci;

INSERT INTO notes (title, body)
VALUES ();
