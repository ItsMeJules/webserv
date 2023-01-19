#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <string>

# include <unistd.h>

# include "Message.hpp"
# include "Client.hpp"
# include "ServerInfo.hpp"
# include "ResponseBuilder.hpp"
# include "utils.hpp"

class HttpResponse : public Message {
	private:
		int _statusCode;
		ws::http_status_t _statusPhrase;

		static ws::http_status_t createStatus(std::string reason, std::string explanation);
	public:
        static std::map<int, ws::http_status_t> codes;

	public:
		HttpResponse();
		HttpResponse(int statusCode);
		HttpResponse(std::string httpVersion, int statusCode);
		HttpResponse(HttpResponse const &htttpResponse);
		~HttpResponse();

		std::string build();
		void send(Client &client);

		HttpResponse &operator=(HttpResponse const &rhs);

        static HttpResponse fromRequest(ServerInfo const &serverInfo, HttpRequest const &request);
        static std::map<int, ws::http_status_t> createCodes();
};

#endif