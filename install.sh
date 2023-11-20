#!/bin/bash
## LoadLinker installer script for Linux ##

CONFIG=$1
VERSION="1.0.1"
NGINX_V="conf.d"

YUM_CMD=$(which yum)
APT_CMD=$(which apt)
APK_CMD=$(which apk)

echo "Installing dependencies..."
if [[ ! -z $YUM_CMD ]]; then
   yum install -y gcc gcc-c++ clang cmake make
elif [[ ! -z $APT_CMD ]]; then
   apt install -y build-essential clang cmake make
elif [[ ! -z $APK_CMD ]]; then
    apk add --no-cache build-base clang cmake make
else
   echo "Error: can't install dependencies!"
   exit 1;
fi

echo "Building LoadLinker v$VERSION..."
cd /tmp && wget https://github.com/lucien-carneiro/LoadLinker/archive/refs/tags/v$VERSION.tar.gz
tar -xvzf v$VERSION.tar.gz && cd LoadLinker-$VERSION
mkdir build && cd build
cmake -G "Unix Makefiles" .. && cmake --build .

if [ "$CONFIG" == "server" ]; then
    echo "Installing LoadLinker server with Nginx..."
    if [[ ! -z $YUM_CMD ]]; then yum install -y nginx; elif [[ ! -z $APT_CMD ]]; then apt install -y nginx; else apk add nginx; fi
    sudo mkdir /etc/loadlinker && sudo cp ../loadlinker/* /etc/loadlinker/ && sudo rm /etc/loadlinker/loadlinker.service
    sudo cp ./LoadLinker /usr/bin/LoadLinker

    echo "Configuring Nginx..."
    if [[ -e "/etc/nginx/http.d/default.conf" ]]; then  $NGINX_V="http.d"; fi
    sudo rm /etc/nginx/$NGINX_V/default.conf
    sudo mv /etc/loadlinker/default.conf /etc/nginx/$NGINX_V/

    echo "Enabling LoadLinker service..."
    sudo mv /etc/loadlinker/loadlinker.service /etc/systemd/system/
    sudo systemctl daemon-reload && sudo systemctl enable loadlinker.service
    sudo systemctl restart nginx && sudo systemctl start loadlinker.service

elif [ "$CONFIG" == "agent" ]; then
    echo "Installing LoadLinker agent..."
    sed '7s/$/Agent/; 2s/^......................./&Agent/' ../loadlinker/loadlinker.service > ./loadlinkeragent.service
    sudo mkdir /etc/loadlinker && sudo cp ../loadlinker/loadlinker.conf /etc/loadlinker/
    sudo cp ./LoadLinkerAgent /usr/bin/LoadLinkerAgent

    echo "Enabling LoadLinkerAgent service..."
    sudo mv ./loadlinkeragent.service /etc/systemd/system/
    sudo systemctl daemon-reload && sudo systemctl enable loadlinkeragent.service
    sudo systemctl start loadlinkeragent.service
fi

cd /tmp && rm -rf v$VERSION.tar.gz LoadLinker-$VERSION
echo "LoadLinker v$VERSION installed successfully!"