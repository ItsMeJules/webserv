# include "ServerSocket.hpp"
# include "Server.hpp"
# include "EPoll.hpp"

int main() {
	ServerSocket serverSocket;
	serverSocket.setup();

	IPoll *poller = new EPoll();
	Server server(serverSocket, poller);

	while (poller->polling(server) > 0) {
	}
    serverSocket.close(poller);
	delete poller;
}