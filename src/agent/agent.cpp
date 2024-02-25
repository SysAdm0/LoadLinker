#include "agent/agent.hpp"

void signal_handler(int signal) {
    gSignalStatus = signal;
}

int main(const int argc, const char *argv[]) {
    config config;
    session session;

    std::signal(SIGINT, signal_handler);
    std::signal(SIGTERM, signal_handler);
    std::signal(SIGQUIT, signal_handler);

    config.loadlinker_configure(AGENT_CONFIG, argc, argv);
    while (gSignalStatus != SIGINT && gSignalStatus != SIGTERM && gSignalStatus != SIGQUIT) {
        if (session.connect_server(config.get_agent_config()) == 0) {
            session.start_session(1);
            session.close_session();
        } else {
            std::cout << "Connection failed" << std::endl;
            sleep(2);
        }
    }
    return 0;
}