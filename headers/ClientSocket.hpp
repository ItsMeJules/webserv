#ifndef CLIENTSOCKET_HPP
# define CLIENTSOCKET_HPP

# include <sys/types.h>
# include <sys/socket.h>

# include "Socket.hpp"

class ClientSocket : public Socket {
	private:
		int _serverListeningFd;
	public:
		ClientSocket();
		ClientSocket(int serverListeningFd);
		ClientSocket(ClientSocket const &clientSocket);
		~ClientSocket();

		bool setup();

		const int &getServerListeningFd() const;

		ClientSocket &operator=(ClientSocket const &rhs);
};

#endif