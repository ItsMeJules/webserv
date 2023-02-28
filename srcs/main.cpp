#include <iostream>

#include "ServerSocket.hpp"
#include "Server.hpp"
#include "config_parser.hpp"
#include "utils.hpp"

#ifdef __linux__
# include "EPoll.hpp"
#else
# include "Poll.hpp"
#endif

#ifdef __linux__
	IPoll *Server::poller = new EPoll(); // EPOLL_LINUX
#else
	IPoll *Server::poller = new Poll(); // POLL_MAC
#endif

std::vector<Server*> Server::servers = std::vector<Server*>();

std::vector<Server *> vecserv;

namespace ws {

    bool setup_servers() {
        bool success = true;

        for (std::vector<Server*>::iterator it = Server::servers.begin(); it != Server::servers.end(); it++) {
            Server *server = *it;

            ws::checkConfiguration(server);
            
            server->getServerSocket().setIp(server->getServerInfo().getIp());
            server->getServerSocket().setPort(server->getServerInfo().getPort());
            success = server->getServerSocket().setup() && server->setup();
        }
        return success;
    }

}

int main(int ac, char **av) {
    bool stop = false;

    if (ac < 2) {
        ws::log(ws::LOG_LVL_ERROR, "[MAIN] -", "You must specify a configuration file!");
        return 1;
    }

    ws::parseConfig(std::string(av[1]), Server::servers);
    if (!ws::setup_servers()) {
        ws::log(ws::LOG_LVL_ERROR, "[MAIN] -", "Stopping program...");
        return 1;
    }

    while (!stop) {
        for (std::vector<Server*>::iterator it = Server::servers.begin(); it != Server::servers.end(); it++) {
            Server *server = *it;

            if (Server::poller->polling(*server) < 0) {
                ws::log(ws::LOG_LVL_ERROR, "[MAIN] -", "something went wrong with server: " + server->getServerInfo().getServerName());
                ws::log(ws::LOG_LVL_ERROR, "[MAIN] -", "Stopping program...");
                delete server;
                stop = true;
                break;
            }
        }
    }
    HttpRequest::clearMethods();
}