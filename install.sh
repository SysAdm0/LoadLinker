#!/bin/bash

CONFIG=$1
VERSION="v1.0.2"
NGINX_CONF="conf.d"

echo "##### LOADLINKER CONFIGURATION #####"

if [ "$CONFIG" == "server" ]; then
    if [[ $(sudo &> /dev/null)  ]]; then
        echo "You don't have sudo right."
	     exit 1
    elif [[ ! $(sudo which nginx) ]]; then
        echo "Nginx is not installed. Please install Nginx first."
        exit 1
    fi

    echo -n "Listen port (default: 50000): "
    read listen_port
    listen_port=${listen_port:-50000}
    echo -n "IP to bind (default: 0.0.0.0): "
    read ip_to_bind
    ip_to_bind=${ip_to_bind:-0.0.0.0}
    echo -n "Upstream path (default: /etc/loadlinker): "
    read upstream
    upstream=${upstream:-/etc/loadlinker}

    echo "Downloading source files..."
    mkdir -p /tmp/loadlinker-$VERSION && sudo mkdir $upstream
    wget "https://github.com/SysAdm0/LoadLinker/releases/download/$VERSION/loadlinker-server" -O /tmp/loadlinker-server
    wget -q "https://github.com/SysAdm0/LoadLinker/archive/refs/tags/$VERSION.tar.gz" -O /tmp/$VERSION.tar.gz
    chmod +x /tmp/loadlinker-server && sudo mv /tmp/loadlinker-server /usr/bin/loadlinker-server
    tar -xf /tmp/$VERSION.tar.gz -C /tmp/loadlinker-$VERSION --strip-components 1

    echo "Configuring Nginx and checking permissions..."
    if [[ -e "/etc/nginx/http.d/default.conf" ]]; then  $NGINX_CONF="http.d"; fi
    sudo mv /tmp/loadlinker-$VERSION/loadlinker/upstream.conf $upstream/upstream.conf
    sudo rm -f /etc/nginx/sites-available/default && sudo rm -f /etc/nginx/sites-enabled/default
    sudo mv /tmp/loadlinker-$VERSION/loadlinker/default.conf /etc/nginx/$NGINX_CONF/
    sudo sed -i "s|/etc/loadlinker|$upstream|" /etc/nginx/$NGINX_CONF/default.conf

    echo "Enabling LoadLinker service..."
    sudo mv /tmp/loadlinker-$VERSION/loadlinker/loadlinker.service /etc/systemd/system/
    sudo sed -i "s|^ExecStart=.*$|ExecStart=/usr/bin/loadlinker-server -p $listen_port -i $ip_to_bind -u $upstream|" \
      /etc/systemd/system/loadlinker.service
    sudo sh -c 'systemctl daemon-reload && systemctl enable loadlinker.service && systemctl start loadlinker.service'

elif [ "$CONFIG" == "agent" ]; then
    echo -n "Server IP: "
    read server_ip
    echo -n "Server port (default: 50000): "
    read server_port
    server_port=${server_port:-50000}
    echo -n "Application port (default: 8080): "
    read app_port
    app_port=${app_port:-8080}

    echo "Downloading source files..."
    mkdir -p /tmp/loadlinker-$VERSION
    wget "https://github.com/SysAdm0/LoadLinker/releases/download/$VERSION/loadlinker-agent" -O /tmp/loadlinker-agent
    wget -q "https://github.com/SysAdm0/LoadLinker/archive/refs/tags/$VERSION.tar.gz" -O /tmp/$VERSION.tar.gz
    chmod +x /tmp/loadlinker-agent && sudo mv /tmp/loadlinker-agent /usr/bin/loadlinker-agent
    tar -xf /tmp/$VERSION.tar.gz -C /tmp/loadlinker-$VERSION --strip-components 1

    echo "Enabling LoadLinker service..."
    sudo mv /tmp/loadlinker-$VERSION/loadlinker/loadlinker.service /etc/systemd/system/
    sudo sed -i "s|^ExecStart=.*$|ExecStart=/usr/bin/loadlinker-agent -p $server_port -ip $server_ip -ap $app_port|" \
      /etc/systemd/system/loadlinker.service
    sudo sh -c 'systemctl daemon-reload && systemctl enable loadlinker.service && systemctl start loadlinker.service'

else
    echo "Please specify 'server' or 'agent' as an argument."
    echo "Usage examples:"
    echo "   ./install.sh server"
    echo "   ./install.sh agent"
    exit 1
fi

echo "Cleaning build files..."
rm -rf /tmp/$VERSION.tar.gz /tmp/LoadLinker-$VERSION
echo "LoadLinker-$CONFIG $VERSION installed successfully!"