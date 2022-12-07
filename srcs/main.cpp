#include "Socket.hpp"
#include "Server.hpp"

int main() {
	Socket socket;

	socket.generateFd();
	socket.setReusable();
	socket.setNonBlocking();
	socket.bindTo(9999);

	Server server(socket);
	
	server.listenOnSocket(10);
	server.initServer();
	server.monitorAllFd();
}