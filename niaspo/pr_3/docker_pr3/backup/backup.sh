#!/usr/bin/env bash
#
# Автоматическое резервное копирование базы MySQL.
# Контейнер работает как демон: делает дамп, чистит старые копии и засыпает.
#
# Переменные окружения:
#   DB_HOST, DB_PORT, DB_NAME, DB_USER, DB_PASSWORD — параметры подключения
#   BACKUP_DIR              — каталог для копий (том backup_data, по умолчанию /backup)
#   BACKUP_KEEP             — сколько последних копий хранить (по умолчанию 7)
#   BACKUP_INTERVAL_SECONDS — период между копиями в секундах (по умолчанию 86400)
#   BACKUP_RUN_ONCE         — 1: сделать один дамп и выйти (ручной запуск)
#
set -euo pipefail

DB_HOST="${DB_HOST:-db}"
DB_PORT="${DB_PORT:-3306}"
DB_NAME="${DB_NAME:-notes}"
DB_USER="${DB_USER:-root}"
DB_PASSWORD="${DB_PASSWORD:-}"
BACKUP_DIR="${BACKUP_DIR:-/backup}"
BACKUP_KEEP="${BACKUP_KEEP:-7}"
BACKUP_INTERVAL_SECONDS="${BACKUP_INTERVAL_SECONDS:-86400}"
BACKUP_RUN_ONCE="${BACKUP_RUN_ONCE:-0}"

# Пароль передаём через переменную окружения клиента MySQL:
# так он не попадает в список процессов (ps) и не вызывает предупреждение CLI.
export MYSQL_PWD="$DB_PASSWORD"

log() { echo "[$(date '+%Y-%m-%d %H:%M:%S')] $*"; }

wait_for_db() {
    local attempt=1
    while ! mysqladmin ping -h "$DB_HOST" -P "$DB_PORT" -u "$DB_USER" --silent >/dev/null 2>&1; do
        if [ "$attempt" -ge 30 ]; then
            log "MySQL ($DB_HOST:$DB_PORT) недоступен, попыток: $attempt" >&2
            return 1
        fi
        log "ожидание MySQL ($attempt/30)"
        attempt=$((attempt + 1))
        sleep 2
    done
    log "MySQL доступен: $DB_HOST:$DB_PORT"
}

make_backup() {
    local ts file
    mkdir -p "$BACKUP_DIR"
    ts="$(date '+%Y%m%d-%H%M%S')"
    file="${BACKUP_DIR}/${DB_NAME}-${ts}.sql.gz"

    log "дамп базы ${DB_NAME} → ${file}"
    if mysqldump -h "$DB_HOST" -P "$DB_PORT" -u "$DB_USER" \
            --single-transaction --quick --routines --events --triggers \
            --databases "$DB_NAME" | gzip -9 > "$file"; then
        if gzip -t "$file"; then
            log "копия создана, размер: $(du -h "$file" | cut -f1)"
        else
            log "архив повреждён, удаляю" >&2
            rm -f "$file"
            return 1
        fi
    else
        log "mysqldump завершился с ошибкой" >&2
        rm -f "$file"
        return 1
    fi
}

prune_backups() {
    local total
    total="$(find "$BACKUP_DIR" -maxdepth 1 -type f -name '*.sql.gz' | wc -l)"
    if [ "$total" -le "$BACKUP_KEEP" ]; then
        return 0
    fi
    # имена копий содержат метку времени YYYYMMDD-HHMMSS,
    # поэтому обратная лексикографическая сортировка = от новых к старым
    find "$BACKUP_DIR" -maxdepth 1 -type f -name '*.sql.gz' | sort -r |
        tail -n +"$((BACKUP_KEEP + 1))" |
        while read -r old; do
            log "удаляю старую копию: $(basename "$old")"
            rm -f "$old"
        done
}

wait_for_db

status=0
while :; do
    if make_backup; then
        :
    else
        status=1
        log "резервное копирование не удалось, продолжаю работу" >&2
    fi
    prune_backups

    if [ "$BACKUP_RUN_ONCE" = "1" ]; then
        log "режим одного запуска (BACKUP_RUN_ONCE=1), выход (код $status)"
        exit "$status"
    fi

    log "следующая копия через ${BACKUP_INTERVAL_SECONDS} с"
    sleep "$BACKUP_INTERVAL_SECONDS"
done
