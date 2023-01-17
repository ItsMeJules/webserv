#include "ServerSocket.hpp"
#include "Server.hpp"

#ifdef __linux__
# include "EPoll.hpp"
#else
# include "Poll.hpp"
#endif

int main() {
	ServerSocket serverSocket;
	serverSocket.setup();
	
#ifdef __linux__
	IPoll *poller = new EPoll(); // EPOLL_LINUX
#else
	IPoll *poller = new Poll(); // POLL_MAC
#endif

	Server server(serverSocket, poller);

	while (poller->polling(server) > 0) {
	}
    poller->deleteFd(serverSocket.getFd());
    close(serverSocket.getFd());
	delete poller;
}