#!/usr/bin/env bash
#
# Диагностика «сайт не открывается на localhost:8080».
# Разделяет четыре разные причины:
#   1) контейнер web не работает;
#   2) мешает прокси из окружения (http_proxy) — curl/browser уходят не туда;
#   3) Apache отвечает внутри сети, но хост не достаёт контейнер (nftables/firewall/VPN);
#   4) localhost резолвится в IPv6 ::1, где никто не слушает.
#
# Использование:
#   ./net-check.sh              обычная проверка
#   ./net-check.sh --port-test  дополнительно проверить публикацию портов на чистом образе
#
set -uo pipefail
cd "$(dirname "$(readlink -f "$0")")"

# shellcheck disable=SC1091
[ -f .env ] && { set -a; . ./.env; set +a; }
PORT="${WEB_PORT:-8080}"

# Локальные адреса не должны уходить в прокси из окружения.
export no_proxy="localhost,127.0.0.1,::1,${no_proxy:-}"
export NO_PROXY="$no_proxy"

say()  { printf '\n\033[1m== %s ==\033[0m\n' "$*"; }
ok()   { printf '  \033[32m✓\033[0m %s\n' "$*"; }
bad()  { printf '  \033[31m✗\033[0m %s\n' "$*"; }
note() { printf '  \033[33m!\033[0m %s\n' "$*"; }
code_of() { local c; c=$(curl -s -o /dev/null -m 6 -w '%{http_code}' "$1" 2>/dev/null); printf '%s' "${c:-000}"; }

say "1. Контейнеры"
docker compose ps

say "2. Кто слушает порт $PORT на хосте"
if ss -tulpn 2>/dev/null | grep -q ":$PORT "; then
    ss -tulpn | grep ":$PORT "
    ok "порт $PORT слушается (обычно это docker-proxy)"
else
    bad "порт $PORT никто не слушает — контейнер web не запущен или порт не проброшен"
fi

say "3. Прокси в окружении"
proxy_env="${http_proxy:-}${HTTP_PROXY:-}${https_proxy:-}${HTTPS_PROXY:-}${all_proxy:-}${ALL_PROXY:-}"
if [ -n "$proxy_env" ]; then
    note "задан прокси: $(env | grep -iE '^(http|https|all|ftp)_proxy=' | tr '\n' ' ')"
    note "curl и браузер могут отправлять запросы к localhost в прокси — отсюда «Connection reset»."
    note "Скрипт обходит это через no_proxy=localhost,127.0.0.1 (проверки ниже уже без прокси)."
else
    ok "переменные *_proxy не заданы"
fi

say "4. Ответ с хоста"
h_ipv4=$(code_of "http://127.0.0.1:$PORT/")
h_name=$(code_of "http://localhost:$PORT/")
printf '  127.0.0.1:%s → %s\n  localhost:%s  → %s\n' "$PORT" "$h_ipv4" "$PORT" "$h_name"

say "5. Ответ напрямую по IP контейнера (проверка, виноват ли docker-proxy)"
web_ip=$(docker inspect -f '{{range .NetworkSettings.Networks}}{{.IPAddress}}{{end}}' pr3_web 2>/dev/null || true)
c_ip="нет данных"
if [ -n "$web_ip" ]; then
    c_ip=$(code_of "http://$web_ip:80/")
    printf '  %s:80 → %s\n' "$web_ip" "$c_ip"
fi

say "6. Apache внутри сети (минуя проброс портов)"
inside=$(docker compose exec -T db bash -c \
    'timeout 8 bash -c "exec 3<>/dev/tcp/web/80 && printf \"GET / HTTP/1.0\r\nHost: localhost\r\n\r\n\" >&3 && head -n 1 <&3"' 2>/dev/null | tr -d '\r')
inside_ok=0
case "$inside" in *200*) inside_ok=1; ok "изнутри сети Apache отдаёт: $inside";; *) [ -n "$inside" ] && bad "странный ответ изнутри: $inside" || bad "Apache не ответил даже внутри сети";; esac

say "7. Сеть Docker и хоста"
printf '  net.ipv4.ip_forward = %s (должно быть 1)\n' "$(sysctl -n net.ipv4.ip_forward 2>/dev/null || echo '?')"
docker info 2>/dev/null | grep -iE 'iptables|nftables|warning' | sed 's/^/  /' || true
if command -v nft >/dev/null 2>&1; then
    cnt=$(sudo nft list ruleset 2>/dev/null | grep -c "dport $PORT" || true)
    printf '  правил nftables для порта %s: %s\n' "$PORT" "${cnt:-0}"
fi
for svc in ufw firewalld wg-quick@wg0; do
    command -v "$svc" >/dev/null 2>&1 && note "обнаружен $svc — проверь, что он не блокирует трафик к 172.16.0.0/12"
done
command -v wg >/dev/null 2>&1 && [ -n "$(wg show 2>/dev/null)" ] && note "активен WireGuard — kill-switch/правила VPN часто ломают доступ к контейнерам"

if [ "${1:-}" = "--port-test" ]; then
    say "8. Тест публикации портов на чистом образе"
    if ss -tulpn 2>/dev/null | grep -q ':18080 '; then
        note "порт 18080 занят, тест пропущен"
    else
        docker run --rm -d --name pr3_porttest -p 18080:80 httpd:2.4 >/dev/null 2>&1
        sleep 2
        pt=$(code_of "http://127.0.0.1:18080/")
        docker rm -f pr3_porttest >/dev/null 2>&1
        if [ "$pt" = "200" ]; then
            ok "чистый контейнер на 18080 отвечает 200 — публикация портов в системе работает"
        else
            bad "чистый контейнер на 18080 тоже не отвечает ($pt) — проблема в Docker/сети хоста, не в проекте"
            note "попробуй: sudo systemctl restart docker; затем повтори тест"
        fi
    fi
fi

say "Итог"
if [ "$h_ipv4" = "200" ] && [ "$h_name" = "200" ]; then
    ok "с хоста сайт отвечает — открывай http://127.0.0.1:$PORT"
    [ -n "$proxy_env" ] && note "если браузер всё равно не открывает — отключи в нём прокси или добавь localhost в исключения"
elif [ "$h_ipv4" = "200" ]; then
    note "по 127.0.0.1 работает, по localhost нет → localhost резолвится в ::1."
    note "Открывай http://127.0.0.1:$PORT или поправь /etc/hosts."
elif [ "$inside_ok" = "1" ] && [ "$c_ip" = "200" ]; then
    note "Apache работает, IP контейнера с хоста доступен, а через docker-proxy — нет."
    note "Пересоздай контейнеры (правила публикации портов протухли после обновления docker):"
    note "  docker compose down && docker compose up -d"
    note "  sudo systemctl restart docker   # если не помогло"
elif [ "$inside_ok" = "1" ]; then
    note "Apache работает внутри сети, но хост не достаёт контейнер по IP ($web_ip)."
    note "Это firewall/VPN на хосте: проверь sudo iptables -S OUTPUT, ufw, wg-quick, kill-switch."
    note "Пока можно проверить работу так: ./net-check.sh --port-test"
else
    bad "Apache не отвечает даже внутри сети — смотри docker compose logs web --tail=50"
fi
