FROM alpine:latest

RUN apk update
RUN apk add nginx
RUN rm -rf /etc/nginx/sites-available/default

COPY ./LoadLinker /usr/bin/LoadLinker
COPY ./config /etc/nginx/loadlinker
COPY ./config/loadlinker.conf /etc/nginx/conf.d/loadlinker.conf

# RUN nginx -s reload

EXPOSE 50000
CMD ["/usr/bin/LoadLinker"]
