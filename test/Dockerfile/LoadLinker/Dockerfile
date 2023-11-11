FROM alpine:latest

RUN apk update
RUN apk add nginx
RUN rm -f /etc/nginx/http.d/default.conf

COPY ./LoadLinker /usr/bin/LoadLinker
COPY ./config /etc/loadlinker
COPY ./config/loadlinker.conf /etc/nginx/http.d/loadlinker.conf

EXPOSE 80
COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh
CMD ["/entrypoint.sh"]
