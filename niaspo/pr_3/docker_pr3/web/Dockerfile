# Образ веб-сервера Apache с собственным сайтом.
# Базовый образ — официальный httpd (Debian, Apache 2.4).
FROM httpd:2.4

LABEL org.opencontainers.image.title="pr3-web" \
      org.opencontainers.image.description="Apache httpd + собственный сайт + reverse proxy на /api/"

# 1. Копируем статический сайт в DocumentRoot образа.
COPY site/ /usr/local/apache2/htdocs/

# 2. Кладём дополнительный конфиг (виртуальный хост + прокси).
COPY site.conf /usr/local/apache2/conf/extra/site.conf

# 3. Подключаем конфиг к основному httpd.conf и проверяем синтаксис.
RUN printf '\n# --- собственный сайт и прокси /api/ ---\nInclude conf/extra/site.conf\n' \
        >> /usr/local/apache2/conf/httpd.conf \
    && apachectl -t

EXPOSE 80
