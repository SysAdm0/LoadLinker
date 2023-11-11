FROM nginx:latest

COPY ./LoadLinkerAgent /usr/bin/LoadLinkerAgent
COPY ./config/network.conf /etc/loadlinker/network.conf
COPY ./config/index.html /usr/share/nginx/html/index.html

COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh
CMD ["/entrypoint.sh"]
