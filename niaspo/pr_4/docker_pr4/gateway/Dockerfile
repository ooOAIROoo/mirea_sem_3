# Контейнер №1 — «шлюз». Слушает localhost:8081 и обращается к localhost:8082.
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
