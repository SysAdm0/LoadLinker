#include "server/server.hpp"

int main(int argc, char **argv) {
    config config;

    config.server_config(argc, argv, 0);
    return 0;
}