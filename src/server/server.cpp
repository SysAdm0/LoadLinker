#include "server/server.hpp"

int main(const int argc, const char *argv[]) {
    config config;
    listener listener;

    config.loadlinker_configure(SERVER_CONFIG, argc, argv);
    if (listener.init_listener(config.get_server_config()) == 0)
        listener.run();
    return 0;
}