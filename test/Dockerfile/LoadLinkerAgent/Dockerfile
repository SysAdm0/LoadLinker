FROM alpine:latest

RUN apk update

COPY ./LoadLinkerAgent /usr/bin/LoadLinkerAgent
COPY ./config/network.conf /etc/loadlinker/network.conf

CMD ["/usr/bin/LoadLinkerAgent"]