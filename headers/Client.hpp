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
		bool _requestFailed;
	public:
		Client();
		Client(ClientSocket socket);
		Client(Client const &client);
		~Client();

		bool init();

		void setSocket(ClientSocket socket);
		void setRequestFailed(bool requestFailed);

		ClientSocket &getSocket();
		RequestParser &getRequestParser();
		HttpRequest &getHttpRequest();
		const bool &hasRequestFailed() const;

		Client &operator=(Client const &rhs);
};

#endif
