#include "server/server.hpp"

int main(int argc, char **argv) {
    config config;
    listener listener;

    config.server_config(argc, argv, 0);
    if (listener.init_listener(config.get_bind_port()) == 0)
        listener.run();
    return 0;
}