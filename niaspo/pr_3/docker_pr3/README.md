# docker_pr3 — стенд для ПР №3 «Создание контейнеров в Docker»

Учебный проект на `docker compose`: собственный сайт на Apache, СУБД MySQL,
своё приложение на Flask и служебный контейнер резервного копирования.

```
docker_pr3/
├── docker-compose.yml        # описание всех четырёх сервисов
├── .env                      # параметры стенда (порты, пароли БД)
├── web/                      # Apache httpd: свой сайт + прокси /api/
│   ├── Dockerfile
│   ├── site.conf             # виртуальный хост и ProxyPass на app:5000
│   └── site/                 # index.html, style.css, app.js
├── db/
│   └── init/01_schema.sql    # создание таблицы notes при первом старте MySQL
├── app/                      # собственное приложение (Flask + PyMySQL)
│   ├── Dockerfile
│   ├── requirements.txt
│   └── app.py
└── backup/                   # служебный контейнер: mysqldump по расписанию
    ├── Dockerfile
    └── backup.sh
```

`verify.sh` — автопроверка стенда, `net-check.sh` — диагностика «сайт не открывается»
(порт, прокси в окружении, доступ по IP контейнера, ответ изнутри сети, firewall).
Флаг `./net-check.sh --port-test` проверяет публикацию портов на чистом образе `httpd`.

## Требования (Arch Linux)

```bash
sudo pacman -Syu docker docker-compose docker-buildx
sudo systemctl enable --now docker
sudo usermod -aG docker "$USER"   # затем перелогин — работа без sudo
```

Пакет `docker-compose` в Arch — это плагин v2, команда вызывается через пробел:
`docker compose …`. Подробный разбор «как это работает, как запускать и чего ожидать» —
в файле [`../howto.md`](../howto.md).

## Запуск

```bash
cd docker_pr3
docker compose up -d --build
docker compose ps
```

Первая сборка занимает 3–10 минут (скачиваются образы, основной — `mysql:8.0` ~600 МБ),
`db` становится `(healthy)` через 30–60 секунд после старта — приложение ждёт её
автоматически.

## Автопроверка

```bash
chmod +x verify.sh
./verify.sh
```

Скрипт проверяет окружение, контейнеры, сайт, API через Apache, запись/чтение в MySQL и
создание резервной копии; в конце печатает `ИТОГ: все проверки пройдены.`

Если сайт не открывается в браузере — запусти диагностику портов:

```bash
./net-check.sh
```

| Сервис   | Что это                          | Доступ                                  |
|----------|----------------------------------|-----------------------------------------|
| `web`    | Apache 2.4 + свой сайт           | <http://localhost:8080>                 |
| `app`    | Flask-приложение (журнал записей)| <http://localhost:5000>, через Apache — `/api/` |
| `db`     | MySQL 8.0, база `notes`          | только внутри сети `pr3_net`            |
| `backup` | дампы БД в том `backup_data`     | `docker compose logs backup`            |

## Проверка

```bash
# сайт
curl -I http://localhost:8080
curl -s http://localhost:8080 | head -n 5

# API приложения (через Apache)
curl -s http://localhost:8080/api/health
curl -s http://localhost:8080/api/notes
curl -s -X POST http://localhost:8080/api/notes \
     -H "Content-Type: application/json" \
     -d '{"title":"Проверка","body":"Запись из curl"}'
```

## Полезные команды

```bash
docker compose logs -f web app db backup   # журналы всех сервисов
docker compose exec db mysql -unotes_app -pnotespass notes \
     -e "SELECT id, title, created_at FROM notes;"
docker compose exec -T db mysqldump -uroot -prootpass notes > manual.sql
docker compose down                        # остановить (данные в томах остаются)
docker compose down -v                     # остановить и удалить тома
```

## Восстановление из резервной копии

```bash
# последняя копия из тома backup_data
docker compose exec backup sh -c 'ls -1t /backup/*.sql.gz | head -n 1'
docker compose exec -T backup sh -c 'gunzip -c $(ls -1t /backup/*.sql.gz | head -n 1)' \
  | docker compose exec -T db mysql -uroot -prootpass
```

## Ручной дамп одним запуском

```bash
docker compose run --rm -e BACKUP_RUN_ONCE=1 backup
```

> Пароли в `.env` — демонстрационные, для учебного стенда. В рабочем окружении
> их хранят в секретах (Docker secrets / переменные CI), а `.env` не коммитят.
