FROM nginx:latest
RUN apt update
RUN apt install -y build-essential binutils cmake clang linux-headers-amd64 make

ARG NAME
ARG VERSION
ARG IP_ADDRESS

ENV NAME=$NAME
ENV VERSION=$VERSION
ENV IP_ADDRESS=$IP_ADDRESS

WORKDIR /tmp/source
COPY ./source .
RUN chmod +x build.sh
RUN sh build.sh

COPY ./config/network.conf /etc/loadlinker/network.conf
COPY ./config/index.sh /tmp/index.sh
RUN cp build/LoadLinkerAgent /usr/bin/LoadLinkerAgent
RUN /tmp/index.sh $NAME $VERSION $IP_ADDRESS

WORKDIR /
COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh
CMD ["/entrypoint.sh"]