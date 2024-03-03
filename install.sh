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
    echo -n "Upstream path (default: ~/.config/loadlinker): "
    read upstream
    upstream=${upstream:-~/.config/loadlinker}

    echo "Downloading source files..."
    mkdir -p ~/.local/bin ~/.config/loadlinker ~/.config/systemd/user /tmp/loadlinker-$VERSION
    wget "https://github.com/SysAdm0/LoadLinker/releases/download/$VERSION/loadlinker-server" -O ~/.local/bin/loadlinker-server
    wget "https://github.com/SysAdm0/LoadLinker/archive/refs/tags/$VERSION.tar.gz" -O /tmp/$VERSION.tar.gz
    chmod +x ~/.local/bin/loadlinker-server
    tar -xvf /tmp/$VERSION.tar.gz -C /tmp/loadlinker-$VERSION --strip-components 1

    echo "Configuring Nginx and checking permissions..."
    if [[ -e "/etc/nginx/http.d/default.conf" ]]; then  $NGINX_CONF="http.d"; fi
    mv /tmp/loadlinker-$VERSION/loadlinker/upstream.conf ~/.config/loadlinker/upstream.conf
    sudo rm -f /etc/nginx/sites-available/default && sudo rm -f /etc/nginx/sites-enabled/default
    sudo mv /tmp/loadlinker-$VERSION/loadlinker/default.conf /etc/nginx/$NGINX_CONF/
    sudo sed -i "s|/etc/loadlinker|$upstream|" /etc/nginx/$NGINX_CONF/default.conf

    echo "Enabling LoadLinker service..."
    mv /tmp/loadlinker-$VERSION/loadlinker/loadlinker.service ~/.config/systemd/user/
    sed -i "s|^ExecStart=.*$|ExecStart=$HOME/.local/bin/loadlinker-server -p $listen_port -i $ip_to_bind -u $upstream|" \
      ~/.config/systemd/user/loadlinker.service
    systemctl --user enable loadlinker.service
    systemctl --user start loadlinker.service

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
    mkdir -p ~/.local/bin ~/.config/systemd/user /tmp/loadlinker-$VERSION
    wget "https://github.com/SysAdm0/LoadLinker/releases/download/$VERSION/loadlinker-agent" -O ~/.local/bin/loadlinker-agent
    wget "https://github.com/SysAdm0/LoadLinker/archive/refs/tags/$VERSION.tar.gz" -O /tmp/$VERSION.tar.gz
    chmod +x ~/.local/bin/loadlinker-agent
    tar -xvf /tmp/$VERSION.tar.gz -C /tmp/loadlinker-$VERSION --strip-components 1

    echo "Enabling LoadLinker service..."
    mv /tmp/loadlinker-$VERSION/loadlinker/loadlinker.service ~/.config/systemd/user/
    sed -i "s|^ExecStart=.*$|ExecStart=$HOME/.local/bin/loadlinker-agent -p $server_port -ip $server_ip -ap $app_port|" \
      ~/.config/systemd/user/loadlinker.service
    systemctl --user enable loadlinker.service
    systemctl --user start loadlinker.service
fi

echo "Cleaning build files..."
rm -rf /tmp/$VERSION.tar.gz /tmp/LoadLinker-$VERSION
echo "LoadLinker-$CONFIG $VERSION installed successfully!"