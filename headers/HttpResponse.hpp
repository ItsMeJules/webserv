#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <string>

# include <unistd.h>

# include "Message.hpp"
# include "Client.hpp"
# include "Server.hpp"

class HttpResponse : public Message {
	private:
		int _statusCode;
		std::string _reasonPhrase;

        static std::map<int, std::string> codes;
	public:
		HttpResponse();
		HttpResponse(std::string httpVersion, int statusCode, std::string reasonPhrase);
		HttpResponse(HttpResponse const &htttpResponse);
		~HttpResponse();

		std::string build();
		void send(Client &client);

		HttpResponse &operator=(HttpResponse const &rhs);

        static HttpResponse fromRequest(Server server, HttpRequest request);
        static std::map<int, std::string> createCodes();
};

#endif