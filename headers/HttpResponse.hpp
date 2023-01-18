#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <string>

# include <unistd.h>

# include "Message.hpp"
# include "Client.hpp"
# include "ServerInfo.hpp"

class HttpResponse : public Message {
	private:
		typedef struct http_error_s {
			std::string reasonPhrase;
			std::string reasonExplanation;
		} http_error_t;
	private:
		int _statusCode;
		std::string _reasonPhrase;

        static std::map<int, http_error_t> codes;
	public:
		HttpResponse();
		HttpResponse(std::string httpVersion, int statusCode, std::string reasonPhrase);
		HttpResponse(HttpResponse const &htttpResponse);
		~HttpResponse();

		std::string build();
		void send(Client &client);

		HttpResponse &operator=(HttpResponse const &rhs);

        static HttpResponse fromRequest(ServerInfo const &serverInfo, HttpRequest const &request);
        static std::map<int, http_error_t> createCodes();
};

#endif