#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>

# include "ClientSocket.hpp"
# include "HttpRequest.hpp"

class Client {
	private:
		ClientSocket _socket;
		HttpRequest _request;
	public:
		Client();
		Client(ClientSocket socket);
		Client(Client const &client);
		~Client();

		bool init();

		void setSocket(ClientSocket socket);
		void setRequest(HttpRequest request);

		ClientSocket &getSocket();
		HttpRequest &getRequest();

		Client &operator=(Client const &rhs);
		friend std::ostream &operator<<(std::ostream &os, const Client &client);
};

#endif