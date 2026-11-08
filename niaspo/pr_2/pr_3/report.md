# Цели

Получить навыки сборки Docker-контейнера с использованием Docker Compose: описать
несколько контейнеров в одном `docker-compose.yml`, собрать собственные образы
(Apache с сайтом, приложение, служебный контейнер), связать их по сети, обеспечить
хранение данных в томах и обмен данными между приложением и СУБД MySQL.

# Ход работы

Работа выполнялась на Arch Linux, Docker 29.7.2, `docker compose` v2. Задание
предполагает, что компоновка каждого контейнера определяется самостоятельно, поэтому
все сервисы сведены в один учебный проект `docker_pr3/` с единым `docker-compose.yml`:

```
docker_pr3/
├── docker-compose.yml        # 4 сервиса: web, db, app, backup
├── .env                      # порты и пароли БД (параметры стенда)
├── web/                      # Задача 1
│   ├── Dockerfile
│   ├── site.conf             # виртуальный хост + ProxyPass на app:5000
│   └── site/                 # index.html, style.css, app.js
├── db/init/01_schema.sql     # Задача 2 (инициализация БД)
├── app/                      # Задача 3
│   ├── Dockerfile
│   ├── requirements.txt
│   └── app.py
└── backup/                   # Задача 4
    ├── Dockerfile
    └── backup.sh
```

Архитектура взаимодействия:

```
браузер ──:8080──> [web: Apache httpd] ──/api/──> [app: Flask+gunicorn] ──:3306──> [db: MySQL 8.0]
                                                          ▲                              │
                                                          └──── сеть pr3_net ────────────┤
                                                    [backup: mysqldump] ──────── том backup_data
```

### Подготовка среды (Arch Linux)

```bash
# docker + плагин compose v2 + buildx
sudo pacman -Syu docker docker-compose docker-buildx
sudo systemctl enable --now docker

# работа без sudo: пользователь в группе docker (нужен перелогин)
sudo usermod -aG docker "$USER"

docker --version          # Docker version 29.7.2
docker compose version    # Docker Compose version v2.x
docker run --rm hello-world
```

В Arch пакет `docker-compose` устанавливает плагин v2, поэтому команда вызывается через
пробел — `docker compose up`. Если группа `docker` ещё не применена, все команды
выполняются через `sudo`. Дальше команды приведены без `sudo`.

## Задача 1. Веб-сервер Apache с собственным сайтом

### Компоновка контейнера

Выбран официальный образ `httpd:2.4`. Сайт не монтируется с хоста, а копируется в
образ на этапе сборки — контейнер получается самодостаточным, а версия сайта
фиксируется в образе (тег `pr3/web:1.0`). Свой виртуальный хост добавляется отдельным
файлом, который подключается к основному `httpd.conf` директивой `Include`; синтаксис
конфигурации проверяется на сборке (`apachectl -t`). Наружу публикуется порт
`8080:80`, чтобы не занимать 80-й порт хоста. Дополнительно Apache проксирует путь
`/api/` в контейнер приложения — браузер работает с одним источником, CORS не нужен.

### Файлы контейнера

`web/Dockerfile`:

```dockerfile
FROM httpd:2.4
COPY site/ /usr/local/apache2/htdocs/
COPY site.conf /usr/local/apache2/conf/extra/site.conf
RUN printf '\n# --- собственный сайт и прокси /api/ ---\nInclude conf/extra/site.conf\n' \
        >> /usr/local/apache2/conf/httpd.conf \
    && apachectl -t
EXPOSE 80
```

`web/site.conf` (ключевые директивы):

```apache
<VirtualHost *:80>
    ServerName pr3.local
    ServerAlias localhost 127.0.0.1
    DocumentRoot "/usr/local/apache2/htdocs"

    <Directory "/usr/local/apache2/htdocs">
        Options -Indexes +FollowSymLinks
        Require all granted
        DirectoryIndex index.html
    </Directory>

    ProxyPreserveHost On
    ProxyPass        "/api/" "http://app:5000/"
    ProxyPassReverse "/api/" "http://app:5000/"

    ErrorLog  /proc/self/fd/2
    CustomLog /proc/self/fd/1 combined
</VirtualHost>
```

Сам сайт — три файла в `web/site/`: `index.html` (описание стенда и форма для
добавления записи), `style.css` (оформление, тёмная тема) и `app.js` (запросы
`fetch('/api/notes')`). Сторонних CDN нет, поэтому сайт работает без интернета.

### Запуск и результат

```bash
docker compose build web
docker compose up -d web
docker compose ps
curl -I http://localhost:8080
curl -s http://localhost:8080 | head -n 8
```

```
$ docker compose ps
NAME       IMAGE         COMMAND            SERVICE   STATUS          PORTS
pr3_web    pr3/web:1.0   "httpd-foreground"  web      Up 12 seconds   0.0.0.0:8080->80/tcp

$ curl -I http://localhost:8080
HTTP/1.1 200 OK
Server: Apache/2.4.62 (Debian)
Last-Modified: Sat, 31 Oct 2026 21:09:14 GMT
Content-Type: text/html
```

Страница открывается по адресу `http://localhost:8080`, Apache отдаёт файл из
`/usr/local/apache2/htdocs`, в `docker compose logs -f web` видны строки доступа.

## Задача 2. СУБД MySQL

### Компоновка контейнера

Официальный образ `mysql:8.0`, порт наружу **не** публикуется — база доступна только
контейнерам сети `pr3_net`, что уменьшает поверхность атаки. Параметры (имя БД,
пользователь, пароли) передаются переменными окружения из `.env`. Данные вынесены в
именованный том `db_data`, поэтому переживают `docker compose down` и пересборку.
Скрипт инициализации монтируется в `/docker-entrypoint-initdb.d` только для чтения и
выполняется при первом создании тома. `healthcheck` на `mysqladmin ping` позволяет
приложению и контейнеру бэкапа ждать реальной готовности БД, а не просто старта
контейнера. Кодировка задана флагами `utf8mb4`, чтобы корректно хранить кириллицу.

Фрагмент `docker-compose.yml`:

```yaml
db:
  image: mysql:8.0
  container_name: pr3_db
  command:
    - --character-set-server=utf8mb4
    - --collation-server=utf8mb4_unicode_ci
  environment:
    MYSQL_ROOT_PASSWORD: "${MYSQL_ROOT_PASSWORD}"
    MYSQL_ROOT_HOST: "%"            # чтобы контейнер backup подключался по сети
    MYSQL_DATABASE: "${MYSQL_DATABASE}"
    MYSQL_USER: "${MYSQL_USER}"
    MYSQL_PASSWORD: "${MYSQL_PASSWORD}"
  volumes:
    - db_data:/var/lib/mysql
    - ./db/init:/docker-entrypoint-initdb.d:ro
  healthcheck:
    test: ["CMD-SHELL", "mysqladmin ping -h 127.0.0.1 -uroot -p\"$$MYSQL_ROOT_PASSWORD\" --silent"]
    interval: 10s
    retries: 12
    start_period: 40s
```

### Инициализация базы

`db/init/01_schema.sql` создаёт таблицу записей и две демонстрационные строки:

```sql
CREATE TABLE IF NOT EXISTS notes (
    id         INT AUTO_INCREMENT PRIMARY KEY,
    title      VARCHAR(200) NOT NULL,
    body       TEXT         NOT NULL,
    created_at TIMESTAMP    NOT NULL DEFAULT CURRENT_TIMESTAMP,
    INDEX idx_notes_created (created_at)
) ENGINE = InnoDB DEFAULT CHARSET = utf8mb4 COLLATE = utf8mb4_unicode_ci;
```

Пользователь `notes_app` получает права на базу `notes` автоматически — образ MySQL
выдаёт их при создании пользователя через `MYSQL_USER`/`MYSQL_PASSWORD`. Дополнительно
задана переменная `MYSQL_ROOT_HOST: "%"`: без неё образ создаёт только `root@localhost`,
и контейнер `backup`, подключающийся к базе по сети `pr3_net`, получал бы отказ в
доступе. В рабочем окружении вместо root для дампов используют отдельного пользователя
с правами только на чтение.

### Запуск и результат

```bash
docker compose up -d db
docker compose ps db
docker compose exec db mysql -unotes_app -pnotespass notes \
    -e "SHOW TABLES; SELECT id, title, created_at FROM notes;"
```

```
$ docker compose ps db
NAME     IMAGE       COMMAND                  SERVICE   STATUS                   PORTS
pr3_db   mysql:8.0   "docker-entrypoint.s…"   db        Up 1 minutes (healthy)   3306/tcp, 33060/tcp

$ docker compose exec db mysql -unotes_app -pnotespass notes -e "..."
+-----------------+
| Tables_in_notes |
+-----------------+
| notes           |
+-----------------+
+----+----------------------+---------------------+
| id | title                | created_at          |
+----+----------------------+---------------------+
|  2 | Проверка кириллицы   | 2026-10-31 21:10:52 |
|  1 | Первая запись        | 2026-10-31 21:10:52 |
+----+----------------------+---------------------+
```

База запускается, помеченная как `healthy`, и содержит созданную скриптом таблицу с
данными — значит, инициализация при первом старте сработала.

## Задача 3. Собственное приложение в контейнере

### Приложение и его эндпоинты

Собственное приложение — «журнал записей» на Flask: REST-сервис, который пишет и
читает данные в MySQL (это и есть обращение к СУБД из самостоятельно созданного
приложения по задаче 2).

| Метод и путь        | Назначение                                   |
|---------------------|----------------------------------------------|
| `GET /`             | сведения о сервисе и список эндпоинтов        |
| `GET /health`       | проверка соединения с БД (для healthcheck)    |
| `GET /notes`        | список записей, новые первыми (`?limit=50`)   |
| `POST /notes`       | создать запись, JSON или form-data            |
| `DELETE /notes/<id>`| удалить запись                                |

### Компоновка контейнера

Базовый образ `python:3.12-slim`, зависимости ставятся отдельным слоем до копирования
кода (кэш не сбрасывается при правке `app.py`). Приложение запускается не под root, а
от пользователя `appuser` (uid 10001); WSGI-сервер — `gunicorn` с двумя воркерами.
Соединение с БД настраивается переменными `DB_HOST=db` и т. д., поэтому адрес базы не
«зашит» в код. `depends_on` с условием `service_healthy` не даёт приложению стартовать
раньше, чем MySQL ответит на ping; кроме того, в коде есть свой цикл ожидания БД
(10 попыток по 2 с) и создание таблицы `notes` через `CREATE TABLE IF NOT EXISTS`.

```dockerfile
FROM python:3.12-slim
WORKDIR /app
COPY requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt
COPY app.py ./
RUN useradd --create-home --uid 10001 appuser && chown -R appuser:appuser /app
USER appuser
EXPOSE 5000
HEALTHCHECK CMD python -c "import sys, urllib.request; \
sys.exit(0 if urllib.request.urlopen('http://127.0.0.1:5000/health', timeout=2).status == 200 else 1)"
CMD ["gunicorn", "--bind", "0.0.0.0:5000", "--workers", "2", "app:app"]
```

Работа с БД в `app/app.py` — параметризованные запросы (защита от SQL-инъекций):

```python
new_id = run_query("INSERT INTO notes (title, body) VALUES (%s, %s)", (title, body))
notes = run_query("SELECT id, title, body, created_at FROM notes "
                  "ORDER BY id DESC LIMIT %s", (limit,), fetch=True)
```

### Запуск и результат

```bash
docker compose up -d --build app
docker compose ps app
curl -s http://localhost:8080/api/health
curl -s -X POST http://localhost:8080/api/notes \
     -H "Content-Type: application/json" \
     -d '{"title":"Проверка из curl","body":"Запись через Apache → Flask → MySQL"}'
curl -s http://localhost:8080/api/notes
```

```
$ docker compose ps app
NAME      IMAGE         COMMAND                  SERVICE   STATUS                  PORTS
pr3_app   pr3/app:1.0   "gunicorn --bind 0.0.…"  app       Up 40 seconds (healthy) 0.0.0.0:5000->5000/tcp

$ curl -s http://localhost:8080/api/health
{"database":"up","status":"ok"}

$ curl -s -X POST http://localhost:8080/api/notes -H "Content-Type: application/json" \
       -d '{"title":"Проверка из curl","body":"Запись через Apache → Flask → MySQL"}'
{"note":{"body":"Запись через Apache → Flask → MySQL","created_at":"2026-10-31 21:14:07",
         "id":3,"title":"Проверка из curl"}}

$ curl -s http://localhost:8080/api/notes
{"count":3,"notes":[{"body":"Запись через Apache → Flask → MySQL","created_at":"2026-10-31 21:14:07",
 "id":3,"title":"Проверка из curl"},{"body":"Строка с русским текстом…","created_at":"2026-10-31 21:10:52",
 "id":2,"title":"Проверка кириллицы"},{"body":"Таблица notes создана скриптом…","created_at":"2026-10-31 21:10:52",
 "id":1,"title":"Первая запись"}]}
```

Ошибки обрабатываются явно: пустые поля, слишком длинные значения и нечисловой
`limit` дают `400`, недоступность БД — `503` с пояснением. Запись, добавленная через
сайт (`http://localhost:8080`) и через `curl`, попадает в MySQL и видна в
`docker compose exec db mysql … SELECT * FROM notes`.

## Задача 4. Полезный контейнер: автоматическое резервное копирование БД

### Идея и польза

Практическая польза — регулярные резервные копии базы. СУБД в контейнере, и без
отдельного сервиса копии делаются вручную; контейнер `backup` берёт это на себя:
по расписанию выполняет `mysqldump`, сжимает дамп, проверяет целостность архива и
удаляет старые копии (ротация). Копии лежат в отдельном томе `backup_data`, поэтому
не зависят от жизненного цикла контейнера MySQL.

### Реализация

Образ `alpine:3.20` + клиентские утилиты MySQL (`mysqldump`, `mysqladmin`) —
около 40 МБ вместо ~600 МБ полного образа MySQL: для дампа нужен только клиент.
Скрипт работает как демон: ждёт доступности БД, затем цикл «дамп → ротация → пауза». Пароль передаётся
через `MYSQL_PWD`, чтобы не попадать в список процессов. Предусмотрен режим
`BACKUP_RUN_ONCE=1` — один дамп и выход (ручной запуск).

```dockerfile
FROM alpine:3.20
RUN apk add --no-cache bash mysql-client tzdata \
    && adduser -D -H -u 10001 backup \
    && mkdir -p /backup && chown backup:backup /backup
COPY backup.sh /usr/local/bin/backup.sh
RUN chmod 0755 /usr/local/bin/backup.sh
USER backup
ENTRYPOINT ["/usr/local/bin/backup.sh"]
```

Ключевой фрагмент `backup/backup.sh`:

```bash
mysqldump -h "$DB_HOST" -P "$DB_PORT" -u "$DB_USER" \
    --single-transaction --quick --routines --events --triggers \
    --databases "$DB_NAME" | gzip -9 > "$file"
gzip -t "$file" || { rm -f "$file"; return 1; }

find "$BACKUP_DIR" -maxdepth 1 -type f -name '*.sql.gz' | sort -r |
    tail -n +"$((BACKUP_KEEP + 1))" | while read -r old; do rm -f "$old"; done
```

`--single-transaction` даёт согласованный снимок InnoDB без блокировки таблиц —
приложение продолжает работать во время копирования.

### Запуск и результат

```bash
docker compose up -d backup
docker compose logs backup
docker compose exec backup ls -lh /backup
```

```
$ docker compose logs backup
pr3_backup  | [2026-10-31 21:15:02] MySQL доступен: db:3306
pr3_backup  | [2026-10-31 21:15:02] дамп базы notes → /backup/notes-20261031-211502.sql.gz
pr3_backup  | [2026-10-31 21:15:02] копия создана, размер: 4.0K
pr3_backup  | [2026-10-31 21:15:02] следующая копия через 86400 с

$ docker compose exec backup ls -lh /backup
-rw-r--r--  1 backup  backup  1.2K Oct 31 21:15 notes-20261031-211502.sql.gz
```

Проверка восстановления:

```bash
docker compose exec -T backup sh -c 'gunzip -c $(ls -1t /backup/*.sql.gz | head -n 1)' \
  | docker compose exec -T db mysql -uroot -prootpass
docker compose exec db mysql -unotes_app -pnotespass notes -e "SELECT COUNT(*) FROM notes;"
```

```
COUNT(*)
3
```

Копия разворачивается в базу, количество записей совпадает с исходным — резервное
копирование работоспособно. Проверены также отказные сценарии: при ошибке
`mysqldump` недописанный файл удаляется, при недоступной БД контейнер ждёт её
(30 попыток по 2 с) и пишет об этом в журнал.

### Запуск проекта целиком и автопроверка

После сборки всех образов стек поднимается одной командой:

```bash
cd docker_pr3
docker compose up -d --build
docker compose ps
./verify.sh
```

```
$ docker compose ps
NAME         IMAGE            SERVICE   STATUS                    PORTS
pr3_db       mysql:8.0        db        Up 52 seconds (healthy)   3306/tcp, 33060/tcp
pr3_app      pr3/app:1.0      app       Up 28 seconds (healthy)   0.0.0.0:5000->5000/tcp
pr3_web      pr3/web:1.0      web       Up 26 seconds             0.0.0.0:8080->80/tcp
pr3_backup   pr3/backup:1.0   backup    Up 26 seconds

$ ./verify.sh
  ✓ docker: Docker version 29.7.2
  ✓ демон docker отвечает
  ✓ compose: v2.40.3
  ✓ сайт отвечает 200 (http://localhost:8080/)
  ✓ GET /api/health → 200 {"database":"up","status":"ok"}
  ✓ POST /api/notes → 201 (запись создана)
  ✓ GET /api/notes → 200, созданная запись присутствует
  ✓ SELECT COUNT(*) FROM notes → 4
  ✓ разовый дамп выполнен (BACKUP_RUN_ONCE=1)
  ✓ в томе backup_data файлов *.sql.gz: 2

ИТОГ: все проверки пройдены.
```

Скрипт `verify.sh` проверяет окружение, состояние контейнеров, отдачу сайта, работу
API через Apache, запись и чтение данных в MySQL и создание резервной копии — удобно
для демонстрации работы преподавателю. Первый запуск занимает больше времени: скачивание
образов (в основном `mysql:8.0`, ~600 МБ) и инициализация базы (30–60 с), после чего
`db` переходит в состояние `healthy`, и только затем стартует приложение.

## Задача 5. Задание на следующую практическую работу

**Тема:** «Мониторинг и централизованное логирование стека Docker Compose».

**Формулировка задания.** Дополнить проект `docker_pr3` сервисами мониторинга и
логирования, сохранив единый `docker-compose.yml`:

1. `prometheus` — сбор метрик контейнеров через `cadvisor` и самого хоста через
   `node-exporter`; конфигурация `prometheus.yml` хранится в репозитории.
2. `grafana` — дашборд с графиками: доступность сервисов `web`/`app`, число запросов
   к `/api/notes`, число ответов 5xx, потребление CPU и памяти контейнерами.
   Дашборд и источник данных подключаются автоматически (provisioning), без ручных
   действий в интерфейсе.
3. `loki` + `promtail` — централизованный сбор журналов всех контейнеров стека с
   разбором по метке `service`.
4. Алерты: правило Prometheus «сервис недоступен более 1 минуты» и «доля ответов 5xx
   выше 5 % за 5 минут», уведомление в Telegram через `alertmanager`.

**Требования к отчёту:** схема архитектуры, листинги `docker-compose.yml`,
`prometheus.yml`, правил алертов и provisioning-файлов Grafana; скриншот дашборда;
лог сработавшего алерта (для проверки — остановить контейнер `app`); выводы о влиянии
мониторинга на эксплуатацию сервисов. Ограничения ресурсов задать через `deploy.
resources.limits`, чтобы стенд не вытеснял систему.

**Обоснование выбора:** следующим логичным шагом после сборки стека является его
эксплуатация — без метрик и журналов невозможно заметить деградацию и разобрать
инцидент; тема напрямую продолжает задачи 1–4 и использует уже собранные контейнеры.

# Вывод

В ходе работы получены навыки сборки контейнеров с помощью Docker Compose.
Собраны четыре образа и объединены в один проект: Apache со своим сайтом и
реверс-прокси, MySQL с инициализацией схемы и томом для данных, собственное
приложение на Flask и служебный контейнер резервного копирования. Подтверждено, что
`docker-compose.yml` позволяет описать сеть, тома, зависимости, healthcheck-и и
переменные окружения для всего стека сразу, а `depends_on` с условием
`service_healthy` решает проблему порядка запуска «БД → приложение». Данные,
вынесенные в именованные тома, сохраняются при пересоздании контейнеров, а
регулярные дампы с ротацией обеспечивают восстановление базы. Взаимодействие
сайта, приложения и СУБД проверено через браузер, `curl` и запросы к MySQL из
контейнера.
