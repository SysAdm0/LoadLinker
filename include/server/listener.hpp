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
        [[nodiscard]] int accept_connection() const;
        [[nodiscard]] std::pair<std::string, int> receive_data(int sock) const;

    private:
        int _sock;
        int _max_simultaneous_connections;
        std::string _upstream_path;
        struct sockaddr_in _host_addr;
        struct timeval _timeout;
};
