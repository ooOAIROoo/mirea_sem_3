# Служебный контейнер резервного копирования MySQL.
# Лёгкий alpine + клиентские утилиты MySQL (mysqldump, mysqladmin).
FROM alpine:3.20

# Каталог /backup создаём в образе, чтобы именованный том
# унаследовал владельца и процесс мог писать от непривилегированного пользователя.
RUN apk add --no-cache bash mysql-client tzdata \
    && adduser -D -H -u 10001 backup \
    && mkdir -p /backup \
    && chown backup:backup /backup

ENV TZ=Europe/Moscow \
    BACKUP_DIR=/backup

COPY backup.sh /usr/local/bin/backup.sh
RUN chmod 0755 /usr/local/bin/backup.sh

USER backup

ENTRYPOINT ["/usr/local/bin/backup.sh"]
