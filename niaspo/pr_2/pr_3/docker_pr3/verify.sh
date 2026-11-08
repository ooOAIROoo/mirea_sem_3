#!/usr/bin/env bash
#
# Автопроверка стенда ПР №3 «Создание контейнеров в Docker».
# Запускать из каталога docker_pr3 после: docker compose up -d --build
#
# Проверяет: окружение → контейнеры → сайт → API → запись в MySQL →
#            резервное копирование. Печатает ✓/✗ по каждому пункту.
#
set -uo pipefail
cd "$(dirname "$(readlink -f "$0")")"

BASE_URL="${BASE_URL:-http://localhost:8080}"
FAILED=0
TMPDIR_CHECK="$(mktemp -d)"
trap 'rm -rf "$TMPDIR_CHECK"' EXIT

ok()    { printf '  \033[32m✓\033[0m %s\n' "$*"; }
bad()   { printf '  \033[31m✗\033[0m %s\n' "$*"; FAILED=$((FAILED + 1)); }
title() { printf '\n\033[1m== %s ==\033[0m\n' "$*"; }

# --- параметры БД из .env -----------------------------------------------------
if [ -f .env ]; then
    # shellcheck disable=SC1091
    set -a; . ./.env; set +a
fi
DB_NAME="${MYSQL_DATABASE:-notes}"
DB_USER="${MYSQL_USER:-notes_app}"
DB_PASS="${MYSQL_PASSWORD:-notespass}"

# --- 1. окружение -------------------------------------------------------------
title "1. Окружение"
if command -v docker >/dev/null 2>&1; then
    ok "docker: $(docker --version 2>/dev/null)"
else
    bad "docker не найден в PATH — sudo pacman -S docker"
fi
if docker info >/dev/null 2>&1; then
    ok "демон docker отвечает"
else
    bad "демон docker недоступен — sudo systemctl start docker (или нет прав: группа docker)"
fi
if docker compose version >/dev/null 2>&1; then
    ok "compose: $(docker compose version --short 2>/dev/null)"
else
    bad "плагин compose не найден — sudo pacman -S docker-compose"
fi
if command -v curl >/dev/null 2>&1; then
    ok "curl найден"
else
    bad "curl не найден — sudo pacman -S curl"
fi

if [ "$FAILED" -ne 0 ]; then
    printf '\nОкружение не готово, дальнейшие проверки отменены.\n'
    exit 1
fi

# --- 2. контейнеры ------------------------------------------------------------
title "2. Контейнеры проекта"
docker compose ps

# --- 3. ожидание сайта --------------------------------------------------------
title "3. Ожидание ответа сайта (до 180 с)"
deadline=$((SECONDS + 180))
code=000
while [ "$SECONDS" -lt "$deadline" ]; do
    code=$(curl -s -o /dev/null -m 3 -w '%{http_code}' "$BASE_URL/" 2>/dev/null || echo 000)
    [ "$code" = "200" ] && break
    sleep 3
done
if [ "$code" = "200" ]; then
    ok "сайт отвечает 200 ($BASE_URL/)"
else
    bad "сайт не отвечает (код $code) — docker compose logs web"
fi

# --- 4. API приложения -------------------------------------------------------
title "4. API приложения через Apache"
health_code=000
deadline=$((SECONDS + 120))
while [ "$SECONDS" -lt "$deadline" ]; do
    health_code=$(curl -s -o "$TMPDIR_CHECK/health.json" -m 5 -w '%{http_code}' \
        "$BASE_URL/api/health" 2>/dev/null || echo 000)
    [ "$health_code" = "200" ] && break
    sleep 3
done
if [ "$health_code" = "200" ]; then
    ok "GET /api/health → 200 $(cat "$TMPDIR_CHECK/health.json")"
else
    bad "GET /api/health → $health_code (приложение не видит MySQL?) — docker compose logs app"
fi

STAMP="$(date '+%Y-%m-%d %H:%M:%S')"
post_code=$(curl -s -o "$TMPDIR_CHECK/post.json" -m 5 -w '%{http_code}' \
    -X POST "$BASE_URL/api/notes" \
    -H 'Content-Type: application/json' \
    -d "{\"title\":\"verify.sh\",\"body\":\"Автопроверка $STAMP\"}" 2>/dev/null || echo 000)
if [ "$post_code" = "201" ]; then
    ok "POST /api/notes → 201 (запись создана)"
else
    bad "POST /api/notes → $post_code"
fi

list_code=$(curl -s -o "$TMPDIR_CHECK/list.json" -m 5 -w '%{http_code}' \
    "$BASE_URL/api/notes" 2>/dev/null || echo 000)
if [ "$list_code" = "200" ] && grep -q 'verify.sh' "$TMPDIR_CHECK/list.json"; then
    ok "GET /api/notes → 200, созданная запись присутствует"
else
    bad "GET /api/notes → $list_code, запись не найдена"
fi

# --- 5. чтение напрямую из MySQL ---------------------------------------------
title "5. Чтение из MySQL внутри контейнера db"
if docker compose exec -T db mysql -u"$DB_USER" -p"$DB_PASS" "$DB_NAME" \
        -N -e "SELECT COUNT(*) FROM notes;" >"$TMPDIR_CHECK/count.txt" 2>"$TMPDIR_CHECK/mysql.err"; then
    ok "SELECT COUNT(*) FROM notes → $(tr -d '[:space:]' <"$TMPDIR_CHECK/count.txt")"
else
    bad "запрос к MySQL не выполнился: $(tail -n 2 "$TMPDIR_CHECK/mysql.err" | tr '\n' ' ')"
fi

# --- 6. резервное копирование ------------------------------------------------
title "6. Резервное копирование"
if docker compose run --rm -e BACKUP_RUN_ONCE=1 backup >"$TMPDIR_CHECK/backup.log" 2>&1; then
    ok "разовый дамп выполнен (BACKUP_RUN_ONCE=1)"
else
    bad "дамп не выполнен, хвост лога:"
    tail -n 4 "$TMPDIR_CHECK/backup.log" | sed 's/^/      /'
fi
copies=$(docker compose exec -T backup sh -c 'ls -1 /backup/*.sql.gz 2>/dev/null | wc -l' 2>/dev/null | tr -d '[:space:]')
if [ "${copies:-0}" -ge 1 ] 2>/dev/null; then
    ok "в томе backup_data файлов *.sql.gz: $copies"
else
    bad "в /backup нет ни одной копии"
fi

# --- итог --------------------------------------------------------------------
printf '\n'
if [ "$FAILED" -eq 0 ]; then
    printf '\033[32mИТОГ: все проверки пройдены.\033[0m\n'
    exit 0
fi
printf '\033[31mИТОГ: провалено проверок — %s. Логи: docker compose logs --tail=50\033[0m\n' "$FAILED"
exit 1
