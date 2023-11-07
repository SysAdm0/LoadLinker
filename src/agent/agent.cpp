#include "agent/agent.hpp"

int main(int argc, char **argv) {
    config config;
    session session;

    config.server_config(argc, argv, 1);
    while (1) {
        if (session.connect_server(config.get_ip_address(), config.get_bind_port()) == 0) {
            session.start_session(1);
            session.close_session();
        }
    }
    return 0;
}