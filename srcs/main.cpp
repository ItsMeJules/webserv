#include "ServerSocket.hpp"
#include "Server.hpp"
#include "config_parser.hpp"

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
        for (std::vector<Server*>::iterator it = Server::servers.begin(); it != Server::servers.end(); it++)
            delete *it;
    }
    HttpRequest::clearMethods();
}