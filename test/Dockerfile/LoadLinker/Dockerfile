FROM alpine:latest

RUN apk update
RUN apk add nginx
RUN apk add build-base binutils cmake clang linux-headers make
RUN rm -f /etc/nginx/http.d/default.conf

WORKDIR /tmp/source
COPY ./source .
RUN chmod +x build.sh
RUN sh build.sh

RUN cp build/LoadLinker /usr/bin/LoadLinker
COPY ./config /etc/loadlinker
COPY ./config/default.conf /etc/nginx/http.d/test.conf

WORKDIR /
COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh
CMD ["/entrypoint.sh"]
