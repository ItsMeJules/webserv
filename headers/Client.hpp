#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>

# include "ClientSocket.hpp"
# include "HttpRequest.hpp"
# include "RequestParser.hpp"

class Client {
	private:
		ClientSocket _socket;
		RequestParser _parser;
	public:
		Client();
		Client(ClientSocket socket);
		Client(Client const &client);
		~Client();

		bool init();

		void setSocket(ClientSocket socket);

		ClientSocket &getSocket();
		RequestParser &getRequestParser();
		HttpRequest &getHttpRequest();

		Client &operator=(Client const &rhs);
		friend std::ostream &operator<<(std::ostream &os, const Client &client);
};

#endif