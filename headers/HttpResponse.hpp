#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <string>

# include <unistd.h>
# include <sys/time.h>

# include "HttpMessage.hpp"
# include "DefaultBody.hpp"
# include "utils.hpp"
# include "ServerInfo.hpp"

class HttpResponse : public HttpMessage {
	private:
		int _statusCode;
		ws::http_status_t _statusPhrase;

		static ws::http_status_t createStatus(std::string reason, std::string explanation);

	public:
        static std::map<int, ws::http_status_t> codes;

	public:
		HttpResponse();
		HttpResponse(int statusCode);
		HttpResponse(HttpResponse const &htttpResponse);
		~HttpResponse();

		const std::string build() const;
		std::string	generateDate();
		void generateError(int code, const ServerInfo &serverInfo, DefaultBody &body);

		void setStatusCode(int statusCode);

		const int &getStatusCode() const;

		HttpResponse &operator=(HttpResponse const &rhs);

        static std::map<int, ws::http_status_t> createCodes();
};

#endif