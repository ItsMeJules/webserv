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

int main(int ac, char **av) {
    if (ac < 2) {
        ServerSocket serverSocket;
        serverSocket.setup();

        Server server(serverSocket);
        server.setup();

        while (Server::poller->polling(server) > 0) {
        }
        serverSocket.close();
        delete Server::poller;
    } else {
        ws::parse_config(std::string(av[1]), Server::servers);
        for (std::vector<Server*>::iterator it = Server::servers.begin(); it != Server::servers.end(); it++) {
            Server *server = *it;

            server->getSocket().setup();
            server->getSocket().setIp("127.0.0.1");
            server->setup();
        }
        while (1) {
            for (std::vector<Server*>::iterator it = Server::servers.begin(); it != Server::servers.end(); it++) {
                Server *server = *it;

                if (Server::poller->polling(*server) < 0) {
                    ws::log(ws::LOG_LVL_ERROR, "[MAIN] -", "something went wrong with server: " + server->getServerInfo().getServerName());
                    delete server;
                    return 1;
                }
            }
        }
    }
    HttpRequest::clearMethods();
}