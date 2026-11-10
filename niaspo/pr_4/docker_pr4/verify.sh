#!/usr/bin/env bash
# Автопроверка практической работы №4 (часть 1).
# Проверяет окружение, состояние контейнеров и работу цепочки
# пользователь -> контейнер №1 -> контейнер №2 -> контейнер №1 -> пользователь.
# Запуск из каталога docker_pr4: ./verify.sh

set -u
cd "$(dirname "$0")" || exit 1

# не даём локальному прокси из окружения перехватывать запросы к localhost
export no_proxy="localhost,127.0.0.1,::1"
export NO_PROXY="$no_proxy"

GATEWAY_URL="${GATEWAY_URL:-http://localhost:8081}"
COMPUTE_URL="${COMPUTE_URL:-http://localhost:8082}"

pass=0
fail=0

ok()   { printf '  \033[32m✓\033[0m %s\n' "$1"; pass=$((pass + 1)); }
bad()  { printf '  \033[31m✗\033[0m %s\n' "$1"; fail=$((fail + 1)); }
step() { printf '\n\033[1m%s\033[0m\n' "$1"; }

# ---------------------------------------------------------------------------
step "1. Окружение"
if command -v docker >/dev/null 2>&1; then
  ok "docker: $(docker --version)"
  if docker info >/dev/null 2>&1; then ok "демон docker отвечает"; else bad "демон docker не отвечает"; fi
else
  bad "docker не найден"
fi
if docker compose version >/dev/null 2>&1; then ok "compose: $(docker compose version --short 2>/dev/null || echo v2)"; fi

# ---------------------------------------------------------------------------
step "2. Контейнеры"
ps_out="$(docker compose ps --format '{{.Name}}\t{{.Image}}\t{{.Status}}' 2>/dev/null || true)"
for c in pr4_gateway pr4_compute; do
  line="$(printf '%s\n' "$ps_out" | grep -i "$c" || true)"
  if [ -n "$line" ]; then ok "контейнер $c запущен: $(printf '%s' "$line" | cut -f3)"; else bad "контейнер $c не запущен"; fi
done

# ---------------------------------------------------------------------------
step "3. Контейнер №2 слушает localhost:8082"
body="$(curl -sf --noproxy '*' --max-time 5 "$COMPUTE_URL/health" || true)"
if [ -n "$body" ]; then ok "GET $COMPUTE_URL/health → $(printf '%s' "$body" | head -c 120)"; else bad "нет ответа от $COMPUTE_URL"; fi

# ---------------------------------------------------------------------------
step "4. Контейнер №1 слушает localhost:8081"
body="$(curl -sf --noproxy '*' --max-time 5 "$GATEWAY_URL/health" || true)"
if [ -n "$body" ]; then ok "GET $GATEWAY_URL/health → $(printf '%s' "$body" | head -c 120)"; else bad "нет ответа от $GATEWAY_URL"; fi

# ---------------------------------------------------------------------------
step "5. Связь №1 → №2 (диагностика цепочки)"
body="$(curl -sf --noproxy '*' --max-time 5 "$GATEWAY_URL/chain" || true)"
if [ -n "$body" ]; then ok "GET $GATEWAY_URL/chain → $(printf '%s' "$body" | head -c 140)"; else bad "контейнер №1 не видит контейнер №2 по localhost"; fi

# ---------------------------------------------------------------------------
step "6. Полный запрос пользователь → №1 → №2 → №1 → пользователь"
text="Проверка межконтейнерного взаимодействия через localhost"
answer="$(curl -s --noproxy '*' --max-time 10 -X POST "$GATEWAY_URL/api/process" \
          -H 'Content-Type: application/json' \
          -d "{\"text\":\"$text\"}" || true)"
if printf '%s' "$answer" | grep -q '"sha256"'; then
  ok "POST $GATEWAY_URL/api/process → 200, в ответе есть результат обработки из №2"
  printf '%s\n' "$answer" | grep -o '"total_ms":[0-9.]*' | sed 's/^/      /'
  printf '%s\n' "$answer" | grep -o '"words":[0-9]*' | sed 's/^/      /'
else
  bad "цепочка не вернула результат: $answer"
fi

# ---------------------------------------------------------------------------
step "7. Пользовательская страница"
code="$(curl -s -o /dev/null -w '%{http_code}' --noproxy '*' --max-time 5 "$GATEWAY_URL/" || true)"
if [ "$code" = "200" ]; then ok "GET $GATEWAY_URL/ → 200 (страница с формой)"; else bad "GET $GATEWAY_URL/ → $code"; fi

# ---------------------------------------------------------------------------
printf '\n'
if [ "$fail" -eq 0 ]; then
  printf '\033[32mИТОГ: все проверки пройдены (%d).\033[0m\n' "$pass"
  exit 0
else
  printf '\033[31mИТОГ: успешно %d, провалено %d.\033[0m\n' "$pass" "$fail"
  exit 1
fi
