#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "ClientSocket.hpp"

class Client {
	private:
		ClientSocket _socket;
	public:
		Client();
		Client(ClientSocket socket);
		Client(Client const &client);
		~Client();

		bool init();

		ClientSocket &getSocket();
		void setSocket(ClientSocket socket);

		Client &operator=(Client const &rhs);
};

#endif