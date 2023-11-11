FROM nginx:latest
RUN apt update
RUN apt install -y build-essential binutils cmake clang linux-headers-amd64 make

WORKDIR /tmp/source
COPY ./source .
RUN chmod +x build.sh
RUN sh build.sh

RUN cp build/LoadLinkerAgent /usr/bin/LoadLinkerAgent
COPY ./config/network.conf /etc/loadlinker/network.conf
COPY ./config/index.html /usr/share/nginx/html/index.html

WORKDIR /
COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh
CMD ["/entrypoint.sh"]
