# Практическая работа №4. Межконтейнерное взаимодействие docker-контейнеров через localhost

## Цели

Получить навыки создания сетевого соединения и передачи данных между
docker-контейнерами посредством `localhost`: собрать два контейнера, каждый из
которых слушает `localhost` на своём порту, и организовать цепочку
«пользователь → контейнер №1 → контейнер №2 → контейнер №1 → пользователь».

В отчёт включена только первая часть работы — межконтейнерное взаимодействие через
`localhost`. Задания на кластер Kubernetes и на развёртывание стека на cloud-сервере
в настоящем отчёте не рассматриваются.

## Метод решения

Docker по умолчанию помещает контейнер в собственное сетевое пространство имён, и его
`localhost` указывает на сам контейнер, а не на соседний. Чтобы два контейнера видели
общий `localhost`, они запускаются в сети хоста: `network_mode: host` в
`docker-compose.yml` отменяет создание отдельного сетевого пространства имён, и
приложение внутри контейнера слушает порт непосредственно на loopback-интерфейсе
машины. Контейнеры при этом различаются только портами — 8081 у первого и 8082 у
второго, что и требует задание.

Обработчик запроса реализован на Python (Flask + gunicorn). Контейнер №1 получает
запрос от пользователя, обращается к `http://localhost:8082/process` контейнера №2,
дожидается ответа и возвращает его пользователю вместе со служебными данными о вызове.

## Ход работы

### Постановка задания

Создать два docker-контейнера, одновременно слушающих `localhost` на разных портах.
Пользователь обращается к контейнеру №1, тот — к контейнеру №2; контейнер №2
обрабатывает запрос и передаёт результат контейнеру №1, а контейнер №1 — пользователю.
API может быть реализовано на любой технологии.

Схема взаимодействия (рис. 1 задания):

```
[Пользователь] --запрос--> [Контейнер №1] --запрос--> [Контейнер №2]
[Пользователь] <--результат-- [Контейнер №1] <--результат-- [Контейнер №2]
```

### Структура проекта

```
pr_4/
├── report.md                 # настоящий отчёт
├── README.md                 # подробное описание работы стенда
└── docker_pr4/
    ├── docker-compose.yml    # два сервиса, сеть хоста
    ├── .env                  # номера портов
    ├── verify.sh             # автопроверка цепочки
    ├── gateway/              # контейнер №1 — шлюз, localhost:8081
    │   ├── Dockerfile
    │   ├── requirements.txt
    │   ├── app.py
    │   └── templates/index.html
    └── compute/              # контейнер №2 — обработчик, localhost:8082
        ├── Dockerfile
        ├── requirements.txt
        └── app.py
```

### Контейнер №1 — шлюз (localhost:8081)

Шлюз принимает запрос пользователя, передаёт текст в контейнер №2 и возвращает
пользователю составной ответ. Адрес следующего узла задан переменной `NEXT_URL`,
номер порта — переменной `PORT`, поэтому менять порты можно без правки кода.

#### Эндпоинты

| Метод и путь | Назначение |
|---|---|
| `GET /` | страница с полем ввода и кнопкой «Отправить запрос» |
| `GET /health` | собственная живость (healthcheck) |
| `GET /chain` | проверка доступности контейнера №2 по `localhost:8082` |
| `POST /api/process` | основной эндпоинт: приём запроса и вызов контейнера №2 |

#### Ключевой фрагмент `gateway/app.py`

```python
NEXT_URL = os.environ.get("NEXT_URL", "http://localhost:8082")

def call_next(text: str, request_id: str) -> tuple[dict | None, dict]:
    started = time.perf_counter()
    meta = {"url": f"{NEXT_URL}/process", "ok": False}
    try:
        response = requests.post(
            f"{NEXT_URL}/process",
            json={"text": text},
            headers={"X-Request-Id": request_id},
            timeout=UPSTREAM_TIMEOUT,
        )
        meta["status"] = response.status_code
        meta["elapsed_ms"] = round((time.perf_counter() - started) * 1000, 3)
        if response.status_code == 200:
            meta["ok"] = True
            return response.json(), meta
        meta["error"] = response.text[:200]
        return None, meta
    except requests.RequestException as exc:
        meta["elapsed_ms"] = round((time.perf_counter() - started) * 1000, 3)
        meta["error"] = f"{type(exc).__name__}: {exc}"
        return None, meta
```

```python
@app.post("/api/process")
def api_process():
    request_id = uuid.uuid4().hex[:12]
    started = time.perf_counter()
    ...
    result, upstream_meta = call_next(text, request_id)
    if result is None:
        return jsonify(error="контейнер №2 не вернул результат",
                       request_id=request_id, upstream=upstream_meta), 502
    return jsonify(request_id=request_id, gateway={...},
                   upstream=upstream_meta, answer=result,
                   total_ms=round((time.perf_counter() - started) * 1000, 3)), 200
```

Ошибка сети не приводит к падению шлюза: недоступность контейнера №2 превращается в
ответ `502` с описанием причины, а `/chain` — в `503`. Так видно, на каком шаге
цепочка прервалась.

#### `gateway/Dockerfile`

```dockerfile
FROM python:3.12-slim

ENV PYTHONDONTWRITEBYTECODE=1 \
    PYTHONUNBUFFERED=1 \
    PORT=8081 \
    NEXT_URL=http://localhost:8082 \
    SERVICE_NAME="Контейнер №1"

WORKDIR /app
COPY requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt
COPY app.py ./
COPY templates/ ./templates/

RUN useradd --create-home --uid 10001 appuser && chown -R appuser:appuser /app
USER appuser

EXPOSE 8081

HEALTHCHECK --interval=5s --timeout=4s --retries=5 --start-period=10s \
    CMD python -c "import os,urllib.request,sys; sys.exit(0 if urllib.request.urlopen('http://127.0.0.1:'+os.environ['PORT']+'/chain', timeout=3).status == 200 else 1)"

CMD ["sh", "-c", "exec gunicorn --bind 0.0.0.0:${PORT} --workers 1 --access-logfile - app:app"]
```

### Контейнер №2 — обработчик (localhost:8082)

Контейнер №2 с пользователем не общается: он принимает `POST /process` от контейнера
№1, проверяет входные данные, вычисляет статистику текста и его SHA-256 и возвращает
результат. Проверка входа обязательна, так как это граница между двумя сервисами:
пустой или нестроковый `text` и слишком длинная строка дают `400`.

#### Ключевой фрагмент `compute/app.py`

```python
@app.post("/process")
def process():
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
        "processed_at": utc_now(),
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
    return jsonify(result), 200
```

`compute/Dockerfile` устроен так же, как у шлюза, и отличается только значениями
`PORT=8082` и `SERVICE_NAME`.

### `docker-compose.yml`

```yaml
name: docker_pr4

services:
  gateway:
    build:
      context: ./gateway
      dockerfile: Dockerfile
    image: pr4/gateway:1.0
    container_name: pr4_gateway
    environment:
      PORT: "${GATEWAY_PORT:-8081}"
      NEXT_URL: "http://localhost:${COMPUTE_PORT:-8082}"
      SERVICE_NAME: "Контейнер №1"
    network_mode: host
    depends_on:
      compute:
        condition: service_healthy
    restart: unless-stopped

  compute:
    build:
      context: ./compute
      dockerfile: Dockerfile
    image: pr4/compute:1.0
    container_name: pr4_compute
    environment:
      PORT: "${COMPUTE_PORT:-8082}"
      SERVICE_NAME: "Контейнер №2"
    network_mode: host
    restart: unless-stopped
```

`network_mode: host` — ключевая директива работы: контейнер использует сетевое
пространство имён хоста, поэтому `localhost` внутри контейнера совпадает с `localhost`
машины, а два контейнера доступны друг другу по `localhost` и различаются только
портами. `depends_on` с условием `service_healthy` задерживает старт шлюза до тех пор,
пока обработчик не ответит на собственный healthcheck.

### Запуск

```bash
cd pr_4/docker_pr4
docker compose up -d --build
docker compose ps
docker compose logs -f
```

Ожидаемые состояния контейнеров (пример вывода):

```
NAME           IMAGE             SERVICE   STATUS                   PORTS
pr4_compute    pr4/compute:1.0   compute   Up 12 seconds (healthy)
pr4_gateway    pr4/gateway:1.0   gateway   Up 5 seconds (healthy)
```

В режиме `network_mode: host` колонка `PORTS` пуста: проброс портов не нужен, сервисы
слушают порты хоста напрямую.

### Результат

Проверка живости обоих узлов и связи между ними:

```bash
curl -s http://localhost:8082/health
curl -s http://localhost:8081/health
curl -s http://localhost:8081/chain
```

```json
{"status":"ok","service":"Контейнер №2","uptime_sec":61.9}
{"status":"ok","service":"Контейнер №1","uptime_sec":137.5}
{"service":"Контейнер №1","upstream":{"ok":true,"body":{"status":"ok","service":"Контейнер №2","uptime_sec":61.9}}}
```

Полный запрос по цепочке:

```bash
curl -s -X POST http://localhost:8081/api/process \
     -H "Content-Type: application/json" \
     -d '{"text":"Контейнеры общаются через localhost на разных портах"}'
```

```json
{
  "request_id": "040656643c7f",
  "gateway": {"service": "Контейнер №1", "listen": "0.0.0.0:8081", "received_at": "2026-11-10T12:17:49Z"},
  "upstream": {"url": "http://localhost:8082/process", "ok": true, "status": 200, "elapsed_ms": 3.729},
  "answer": {
    "request_id": "040656643c7f",
    "service": "Контейнер №2",
    "processed_at": "2026-11-10T12:17:49Z",
    "result": {"chars": 52, "chars_no_spaces": 46, "words": 7, "unique_words": 7,
               "longest_word": "Контейнеры",
               "sha256": "77cbc8ced99f563512d822e60b060bcc0d55f08d561d7358e0e5d03de64bbc01"},
    "processing_ms": 0.187
  },
  "total_ms": 3.912
}
```

В ответе присутствуют данные обоих контейнеров: блок `gateway` сформирован
контейнером №1, блок `answer` — контейнером №2 (в нём своё поле `service` и своя
метка времени). Значит, запрос действительно прошёл через второй контейнер и вернулся.

Журналы подтверждают два обращения: пользователь → шлюз, затем шлюз → обработчик
(клиент — `python-requests`, то есть запрос сделал контейнер №1, а не пользователь).

```
$ docker compose logs gateway
pr4_gateway  | 127.0.0.1 - - [10/Nov/2026:12:17:49] "GET / HTTP/1.1" 200 5632 "-" "curl/7.88.1"
pr4_gateway  | 127.0.0.1 - - [10/Nov/2026:12:17:49] "POST /api/process HTTP/1.1" 200 737 "-" "curl/7.88.1"

$ docker compose logs compute
pr4_compute  | 127.0.0.1 - - [10/Nov/2026:12:17:49] "POST /process HTTP/1.1" 200 456 "-" "python-requests/2.34.2"
```

Страница пользователя доступна по адресу `http://localhost:8081`: в поле ввода
вводится текст, кнопка «Отправить запрос» обращается к `/api/process`, ниже
выводится JSON-ответ. Отдельная кнопка «Проверить связь с №2» вызывает `/chain`.

### Проверка отказного сценария

Если остановить контейнер №2, шлюз остаётся работоспособным и сообщает, где оборвалась
цепочка:

```bash
docker compose stop compute
curl -s -X POST http://localhost:8081/api/process \
     -H "Content-Type: application/json" -d '{"text":"тест"}'
```

```json
{"error":"контейнер №2 не вернул результат","request_id":"cd9c21523742",
 "service":"Контейнер №1",
 "upstream":{"url":"http://localhost:8082/process","ok":false,"elapsed_ms":1.525,
             "error":"ConnectionError: ... [Errno 111] Connection refused"}}
```

Код ответа — `502`, `GET /chain` в этом состоянии возвращает `503`. После
`docker compose start compute` цепочка восстанавливается без пересборки образов.

### Автопроверка

```bash
cd pr_4/docker_pr4
./verify.sh
```

Скрипт проверяет наличие Docker и плагина Compose, состояние контейнеров, ответы обоих
узлов, связь №1 → №2, полный запрос по цепочке и отдачу страницы пользователя:

```
1. Окружение
  ✓ docker: Docker version 29.7.2
  ✓ демон docker отвечает
  ✓ compose: v2.40.3
2. Контейнеры
  ✓ контейнер pr4_gateway запущен: Up 10 seconds (healthy)
  ✓ контейнер pr4_compute запущен: Up 17 seconds (healthy)
3. Контейнер №2 слушает localhost:8082
  ✓ GET http://localhost:8082/health → {"status":"ok","service":"Контейнер №2",...}
4. Контейнер №1 слушает localhost:8081
  ✓ GET http://localhost:8081/health → {"status":"ok","service":"Контейнер №1",...}
5. Связь №1 → №2 (диагностика цепочки)
  ✓ GET http://localhost:8081/chain → {"service":"Контейнер №1","upstream":{"ok":true,...}}
6. Полный запрос пользователь → №1 → №2 → №1 → пользователь
  ✓ POST http://localhost:8081/api/process → 200, в ответе есть результат обработки из №2
7. Пользовательская страница
  ✓ GET http://localhost:8081/ → 200 (страница с формой)

ИТОГ: все проверки пройдены (9).
```

## Вывод

В ходе работы созданы два docker-контейнера, слушающих `localhost` на разных портах, и
настроена передача данных между ними по цепочке «пользователь → контейнер №1 →
контейнер №2 → контейнер №1 → пользователь». Требуемая схема реализована через
`network_mode: host`: контейнеры используют сетевое пространство имён хоста, поэтому
обращение к `localhost:8082` из первого контейнера доходит до второго, а не до самого
себя. Подтверждено, что при таком режиме каждый контейнер обязан слушать свой порт,
иначе возникает конфликт `Address already in use`. Проверены успешный сценарий (ответ
`200` с данными обоих контейнеров и временем обработки), отказной сценарий при
остановленном втором контейнере (`502`/`503` с описанием ошибки) и восстановление
работы после его запуска. Показано, что режим сети хоста даёт простой доступ по
`localhost`, но лишает контейнеры сетевой изоляции; альтернативой служат
пользовательская bridge-сеть с обращением по имени сервиса и устаревшая опция `--link`.

## Библиографический список

1. Сейерс, Э. Х. Docker на практике / Э. Х. Сейерс, А. Милл ; пер. с англ.
   Д. А. Беликов. — Москва : ДМК Пресс, 2020. — 516 с. — ISBN 978-5-97060-772-5. —
   Текст : электронный // Лань : электронно-библиотечная система. — URL:
   https://e.lanbook.com/book/131719 (дата обращения: 10.11.2026). — Режим доступа:
   для авториз. пользователей.
2. Соединение контейнеров опцией --link : документация Docker. — URL:
   https://docs.docker.com/network/links/ (дата обращения: 10.11.2026).
3. Обзор сетевых возможностей Docker. — URL: https://docs.docker.com/network/
   (дата обращения: 10.11.2026).
