#pragma once
#include "server/server.hpp"

#include <arpa/inet.h>
#include <sys/socket.h>

class listener {
    public:
        listener();
        ~listener() = default;

        int init_listener(std::map<std::string, std::string> config);
        void run();

    protected:
        [[nodiscard]] std::string accept_connection() const;

    private:
        int _sock;
        std::string _upstream_path;
        struct sockaddr_in _host_addr;
        struct timeval _timeout;
};
