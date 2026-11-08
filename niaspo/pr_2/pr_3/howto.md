# Как это работает, как запускать и чего ожидать

Практическая работа №3 «Создание контейнеров в Docker» · Arch Linux + Docker Compose.
Проект: `docker_pr3/`. Отчёт по работе — `report.md`, здесь — разбор «для себя».

---

# 1. Как это работает

## 1.1. Что именно собрано

Четыре контейнера, объединённые в один проект `docker_pr3`:

```
                 ┌─────────────────────────── сеть pr3_net (bridge) ───────────────────────────────┐
                 │                                                                                 │
 браузер ────────┼── :8080 ──► [web]  Apache httpd 2.4                                             │
 (хост)          │              ├─ статика: /usr/local/apache2/htdocs (index.html, css, js)        │
                 │              └─ /api/* ──ProxyPass──► [app]  Flask + gunicorn :5000             │
                 │                                          │                                      │
                 │                                          └── PyMySQL :3306 ──► [db] MySQL 8.0   │
                 │                                                                  │              │
                 │                                            том db_data ◄─────────┘              │
                 │                                                                                 │
                 │                                       [backup] mysqldump ──► том backup_data    │
                 └─────────────────────────────────────────────────────────────────────────────────┘
```

| Сервис   | Образ                 | Внутри сети       | Наружу (хост)      | Данные               |
|----------|-----------------------|-------------------|--------------------|----------------------|
| `web`    | `pr3/web:1.0` (httpd) | `web:80`          | `localhost:8080`   | сайт лежит в образе  |
| `app`    | `pr3/app:1.0` (python)| `app:5000`        | `localhost:5000`   | —                    |
| `db`     | `mysql:8.0`           | `db:3306`         | **не публикуется** | том `db_data`        |
| `backup` | `pr3/backup:1.0`      | —                 | —                  | том `backup_data`    |

## 1.2. Восемь механизмов, на которых всё держится

**1. Один `docker-compose.yml` = один проект.** У проекта есть имя `name: docker_pr3`,
поэтому все контейнеры, сеть и тома получают префикс: `pr3_web`, `docker_pr3_pr3_net`
(контейнеры названы явно через `container_name`). `docker compose` читает файл из
текущего каталога, команды действуют на весь проект сразу.

**2. Сеть и DNS по именам сервисов.** Compose создаёт сеть `pr3_net` и подключает к ней
все сервисы. Внутри неё работает DNS Docker: имя `db` резолвится в IP контейнера `db`,
имя `app` — в IP приложения. Именно поэтому в коде стоит `DB_HOST=db`, а в Apache —
`ProxyPass "/api/" "http://app:5000/"`.

> Главная ловушка новичка: `localhost` **внутри** контейнера — это сам контейнер, а не
> хост и не соседний контейнер. Обращение к БД по `localhost:3306` из `app` даст
> «Connection refused». Внутри сети — только по имени сервиса.

**3. Порты: `ports` vs `EXPOSE`.** `EXPOSE` в Dockerfile — это документация («сервис
слушает 5000»), он ничего не открывает на хосте. Наружу порт публикует только `ports`
в compose: `"8080:80"` = «порт 8080 хоста → порт 80 контейнера». У `db` публикации нет,
поэтому с хоста `mysql -h 127.0.0.1` не подключится — только
`docker compose exec db mysql ...` или из другого контейнера сети. Это осознанное
решение: база не торчит в хост.

**4. Образы и слои.** Каждый сервис с `build:` собирается из своего `Dockerfile`, где
`COPY` копирует файлы внутрь образа. Отсюда важное следствие: **правки в `web/site/*.html`
и `app/app.py` не видны в работающем контейнере** — они попадут туда только после
пересборки (`docker compose up -d --build web`). Плюс подхода: образ самодостаточен, его
можно отдать на другую машину. В `app/Dockerfile` зависимости ставятся до копирования
кода — слой с `pip install` кэшируется и не пересобирается при каждой правке `app.py`.

**5. Тома.** `db_data:/var/lib/mysql` — данные MySQL живут в именованном томе, а не в
слое контейнера. Поэтому `docker compose down` (без `-v`) данные не удаляет, а
`docker compose down -v` — удаляет. Скрипты из `db/init/` выполняются **только при
создании пустого тома**, то есть один раз за жизнь базы. Изменил SQL — либо
`down -v` (всё заново), либо применяй `ALTER` вручную.

**6. `depends_on` + `healthcheck` = порядок запуска.** `depends_on` сам по себе ждёт лишь
*запуска* контейнера, а MySQL стартует 30–60 с. Поэтому у `db` есть `healthcheck`
(`mysqladmin ping` каждые 10 с), а у `app` — `depends_on: db: condition:
service_healthy`: приложение не начнёт старт, пока база не ответит. Дополнительно в
`app.py` есть свой цикл ожидания (10 попыток × 2 с) и `CREATE TABLE IF NOT EXISTS` —
страховка на случай, если том поднялся раньше БД.

**7. Переменные окружения.** `.env` рядом с compose-файлом подхватывается
автоматически, а в `docker-compose.yml` используются подстановки
`${MYSQL_ROOT_PASSWORD}` и `${WEB_PORT:-8080}` (значение по умолчанию после `:-`).
Один и тот же файл описывает и базу, и приложение, и бэкап — меняя пароль, ты меняешь
его в трёх местах сразу.

**8. Реверс-прокси.** Apache отдаёт статику сам, а всё, что начинается с `/api/`,
перебрасывает внутрь `app:5000`. Из-за этого `fetch('/api/notes')` в браузере — это
запрос на тот же origin (`localhost:8080`), CORS не нужен, а порт приложения наружу
можно было бы вообще не публиковать (он открыт только для отладки).

## 1.3. Путь одного запроса (что за что отвечает)

| Шаг | Что происходит | Где это написано |
|-----|----------------|------------------|
| 1 | `GET /` → Apache отдаёт `index.html` | `web/Dockerfile` (COPY site/), `web/site.conf` (DocumentRoot) |
| 2 | Браузер грузит `style.css`, `app.js` | `web/site/` |
| 3 | JS: `fetch('/api/notes')` | `web/site/app.js` |
| 4 | Apache видит `/api/` и проксирует в `app:5000` | `web/site.conf`: `ProxyPass` |
| 5 | Gunicorn передаёт запрос Flask-приложению | `app/Dockerfile`: `CMD gunicorn … app:app` |
| 6 | Приложение делает `SELECT … FROM notes` в `db:3306` | `app/app.py`: `DB_CONFIG`, `run_query` |
| 7 | MySQL отдаёт строки, приложение — JSON | `app/app.py`: `list_notes` |
| 8 | JSON возвращается в браузер, JS перерисовывает таблицу | `web/site/app.js`: `render` |

Так же работает форма: `POST /api/notes` → прокси → Flask → `INSERT` в MySQL → новая
строка в таблице на странице.

---

# 2. Подготовка в Arch Linux

Docker у тебя уже стоит после ПР1 (Docker 29.7.2), но для этой работы нужен ещё
compose-плагин. Полный набор:

```bash
# docker + плагин compose v2 + buildx (сборка по умолчанию)
sudo pacman -Syu docker docker-compose docker-buildx

# автозапуск демона и старт сейчас
sudo systemctl enable --now docker
systemctl status docker --no-pager

# чтобы работать без sudo: добавить себя в группу docker
sudo usermod -aG docker "$USER"
newgrp docker        # применить в текущем терминале (или просто перелогиниться)
```

Проверка, что всё готово:

```bash
docker --version            # Docker version 29.7.2
docker compose version      # Docker Compose version v2.x
docker run --rm hello-world # «Hello from Docker!» — демон работает и образы тянутся
```

> В Arch пакет `docker-compose` ставит **плагин Compose v2**, поэтому рабочая команда —
> `docker compose` (через пробел). Отдельный бинарь `docker-compose` в новых версиях
> может отсутствовать: если привык к дефису, добавь в `~/.bashrc`
> `alias docker-compose='docker compose'`.
>
> Если группа `docker` ещё не применилась (после `usermod` не перелогинился), каждая
> команда потребует `sudo`: `sudo docker compose up -d --build`. Тогда и `verify.sh`
> запускай через `sudo ./verify.sh`.

---

# 3. Как запускать

Все команды — из каталога проекта:

```bash
cd ~/my-obsidian-vault/univ/sem_3/niaspo/pr_3/docker_pr3
```

## Шаг 1. Первая сборка (3–10 минут)

```bash
docker compose build
```

Что увидишь: скачивание базовых образов (`httpd:2.4` ~60 МБ, `python:3.12-slim` ~50 МБ,
`mysql:8.0` ~600 МБ, `alpine` ~10 МБ), затем сборка. У `web` в конце сборки
выполняется `apachectl -t` — ожидаемый вывод `Syntax OK`. Если там `Syntax error` —
ошибка в `web/site.conf`, сборка остановится.

Можно сразу собрать и запустить одной командой: `docker compose up -d --build`.

## Шаг 2. Запуск

```bash
docker compose up -d
docker compose ps
```

Ожидаемо (порядок строк может отличаться):

```
NAME         IMAGE            SERVICE   STATUS                    PORTS
pr3_db       mysql:8.0        db        Up 45 seconds (healthy)   3306/tcp, 33060/tcp
pr3_app      pr3/app:1.0      app       Up 20 seconds (healthy)   0.0.0.0:5000->5000/tcp
pr3_web      pr3/web:1.0      web       Up 18 seconds             0.0.0.0:8080->80/tcp
pr3_backup   pr3/backup:1.0   backup    Up 18 seconds
```

`(healthy)` у `db` появляется через **30–60 секунд** на первом запуске (инициализация
базы) и за ~10–15 с на последующих. Именно столько `app` и будет ждать — это нормально,
а не зависание.

## Шаг 3. Логи (что там должно быть)

```bash
docker compose logs -f db app web backup
```

Ожидаемые ключевые строки:

```
pr3_db      | [System] [MY-013276] ... /var/lib/mysql/
pr3_db      | [Server] /usr/sbin/mysqld: ready for connections. Version: '8.0.xx'
pr3_app     | [INFO] Starting gunicorn 22.0.0
pr3_app     | [INFO] Listening at: http://0.0.0.0:5000 (1)
pr3_web     | AH00558: httpd: Could not reliably determine the server's fully qualified domain name
pr3_web     | [mpm_event:notice] AH00489: Apache/2.4.62 (Debian) configured -- resuming normal operations
pr3_backup  | [2026-10-31 21:15:02] MySQL доступен: db:3306
pr3_backup  | [2026-10-31 21:15:02] дамп базы notes → /backup/notes-20261031-211502.sql.gz
pr3_backup  | [2026-10-31 21:15:02] следующая копия через 86400 с
```

`AH00558` про FQDN — безобидное предупреждение Apache, оно есть почти всегда.

## Шаг 4. Открыть сайт

Браузер: <http://localhost:8080>

Ожидаемо: тёмная страница «Собственный сайт в контейнере Apache», список сервисов,
блок «Журнал записей» с уже двумя строками из `db/init/01_schema.sql` и формой. Заполни
заголовок и текст → «Сохранить в MySQL» → под формой зелёное «Запись №3 сохранена в
MySQL», таблица обновляется. Если вместо этого красная ошибка или пустой список — см.
раздел 5.

## Шаг 5. Проверки из терминала

```bash
# сайт
curl -I http://localhost:8080
# → HTTP/1.1 200 OK, Server: Apache/2.4.62 (Debian)

# связка Apache → Flask → MySQL
curl -s http://localhost:8080/api/health
# → {"database":"up","status":"ok"}

# запись
curl -s -X POST http://localhost:8080/api/notes \
     -H "Content-Type: application/json" \
     -d '{"title":"Проверка из curl","body":"Запись через Apache → Flask → MySQL"}'
# → {"note":{"id":4,"title":"Проверка из curl", ...,"created_at":"…"}}

# чтение
curl -s http://localhost:8080/api/notes
# → {"count":4,"notes":[ … ]}

# и то же самое прямо в базе
docker compose exec db mysql -unotes_app -pnotespass notes \
     -e "SELECT id, title, created_at FROM notes;"
# → таблица со строками (предупреждение про пароль в командной строке — норма)
```

## Шаг 6. Резервная копия

```bash
docker compose logs backup                       # что уже сделал демон
docker compose run --rm -e BACKUP_RUN_ONCE=1 backup   # сделать копию прямо сейчас
docker compose exec backup ls -lh /backup        # посмотреть файлы
```

Ожидаемо: `notes-20261031-211502.sql.gz` размером 1–5 КБ (база маленькая), в логе —
«копия создана» и «следующая копия через 86400 с».

Проверка восстановления (копия разворачивается обратно в базу):

```bash
docker compose exec -T backup sh -c 'gunzip -c $(ls -1t /backup/*.sql.gz | head -n 1)' \
  | docker compose exec -T db mysql -uroot -prootpass
docker compose exec db mysql -unotes_app -pnotespass notes -e "SELECT COUNT(*) FROM notes;"
```

## Шаг 7. Автопроверка всего стенда

В проекте есть `verify.sh` — он сам проверяет окружение, контейнеры, сайт, API,
запись в MySQL и бэкап, и печатает ✓/✗:

```bash
chmod +x verify.sh   # если бит выполнения не сохранился при копировании
./verify.sh
```

Ожидаемый финал: `ИТОГ: все проверки пройдены.` — удобно для показа преподавателю.

## Шаг 8. Остановка и уборка

```bash
docker compose stop            # остановить, контейнеры остаются
docker compose start           # снова запустить (данные на месте)
docker compose down            # удалить контейнеры и сеть, тома с данными остаются
docker compose down -v         # + удалить тома (БД и бэкапы пропадут!)
docker compose restart app     # перезапустить один сервис
```

## Пересборка после правок

| Что правил | Команда |
|------------|---------|
| `app/app.py`, `app/requirements.txt` | `docker compose up -d --build app` |
| `web/site/*` (html/css/js), `web/site.conf` | `docker compose up -d --build web` |
| `docker-compose.yml`, `.env` | `docker compose up -d` |
| `db/init/*.sql` | `docker compose down -v && docker compose up -d --build` (иначе скрипт не выполнится повторно) |

---

# 4. Чего ожидать: тайминги и поведение

| Этап | Что видишь | Время |
|------|-----------|-------|
| `docker compose build` (первый раз) | тянутся образы, `Syntax OK` у web | 3–10 мин (зависит от сети) |
| `docker compose build` (повторно) | всё из кэша | 2–10 с |
| `up -d` → `db (healthy)` | MySQL инициализирует том | 30–60 с первый раз, 10–15 с потом |
| `app` после `db` | ждёт базу, затем gunicorn | +5–15 с |
| `web` | Apache стартует почти сразу | 1–3 с |
| `backup` | первый дамп сразу, потом спит сутки | секунды |
| сайт | страница с 2 записями и формой | сразу после `web` |
| `verify.sh` | ✓ по всем пунктам | 20–60 с |

**Это нормально и не является ошибкой:**

- `AH00558: Could not reliably determine the server's fully qualified domain name` в логах web;
- `mysql: [Warning] Using a password on the command line interface can be insecure` при `exec db mysql -p…`;
- `503 Service Unavailable` на `/api/...` первые секунды после старта — Apache уже поднялся, приложение ещё нет;
- `app` какое-то время в статусе `health` вместо `healthy`, если база стартовала медленно;
- в браузере на `http://localhost:5000` — «сырой» JSON, а не сайт: это API-порт для отладки, сайт живёт на 8080;
- `docker compose ps` показывает `3306/tcp` у базы без `0.0.0.0:` — порт не опубликован, доступ только изнутри сети.

---

# 5. Если что-то пошло не так

| Симптом | Причина | Решение |
|---------|---------|---------|
| `permission denied while trying to connect to the Docker daemon socket` | нет прав на `/var/run/docker.sock` | `sudo usermod -aG docker $USER` + перелогин, либо запускать через `sudo` |
| `Cannot connect to the Docker daemon at unix:///var/run/docker.sock` | демон не запущен | `sudo systemctl start docker` |
| `docker: unknown command: docker compose` / `compose is not a docker command` | не установлен плагин | `sudo pacman -S docker-compose` |
| `Bind for 0.0.0.0:8080 failed: port is already allocated` / `address already in use` | порт занят | `ss -tulpn \| grep -E '8080\|5000'`; поменять `WEB_PORT`/`APP_PORT` в `.env`, затем `docker compose up -d` |
| `503 Service Unavailable` на `/api/...` | Apache жив, приложение нет | `docker compose ps`, `docker compose logs app` — чаще всего `Access denied` к БД |
| `Access denied for user 'notes_app'@'172.x.x.x'` | менял пароль в `.env`, а том со старым уже создан | `docker compose down -v && docker compose up -d --build` (данные пропадут — это учебный стенд) |
| таблицы `notes` нет / старые данные | `db/init/*.sql` работает только на пустом томе | `docker compose down -v` и заново |
| в базе `????` вместо кириллицы | том создан без `utf8mb4` (был старый compose) | `docker compose down -v`, поднять заново с текущими флагами |
| сайт 200, но таблица пустая и в консоли браузера ошибка | не проксируется `/api/` | `docker compose logs web \| grep -i proxy`; проверить `web/site.conf` |
| сборка `web` падает на `apachectl -t` | синтаксис `site.conf` | читать сообщение Apache: оно указывает строку; проверить баланс `<VirtualHost>` |
| сборка `app` падает на `pip install` | сеть/прокси | повторить `docker compose build app`; при VPN — отключить; `--no-cache` при странностях |
| изменения в `app.py`/`index.html` «не применяются» | правки не попали в образ | `docker compose up -d --build app` (или `web`) |
| контейнер `app` циклически перезапускается | ошибка в коде при старте | `docker compose logs app --tail=50` |
| `no space left on device` | старые образы/тома | `docker system df`, затем `docker system prune -a` (без `--volumes`, чтобы не снести базу) |
| `verify.sh: Permission denied` | нет бита выполнения | `chmod +x verify.sh` |

---

# 6. Шпаргалка

```bash
# состояние
docker compose ps                       # контейнеры проекта и их статус
docker compose top                      # процессы внутри
docker compose images                   # образы проекта

# логи
docker compose logs -f                  # все сервисы
docker compose logs -f --tail=50 app    # только приложение

# внутрь контейнера
docker compose exec db bash             # shell в MySQL-контейнере
docker compose exec db mysql -uroot -prootpass -e "SHOW DATABASES;"
docker compose exec app sh              # shell в контейнере приложения (python:3.12-slim)
docker compose exec web cat /usr/local/apache2/conf/extra/site.conf   # наш конфиг внутри образа

# ресурсы и уборка
docker compose stats                    # CPU/память в реальном времени
docker system df                        # сколько занимают образы и тома
docker compose down --remove-orphans    # убрать лишние контейнеры проекта
```

Дальше по плану (задача 5 работы) — добавить в этот же стек Prometheus, Grafana,
cAdvisor и Loki: метрики и логи подключаются к тем же сервисам, ничего переписывать
не придётся.
