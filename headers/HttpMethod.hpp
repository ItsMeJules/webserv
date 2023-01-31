#ifndef HTTPMETHOD_HPP
# define HTTPMETHOD_HPP

# include <string>
# include <fstream>

# include <sys/stat.h>
# include <sys/types.h>

# include "ServerInfo.hpp"
# include "HttpResponse.hpp"
# include "utils.hpp"

class HttpRequest;

class HttpMethod {
	protected:
		ws::request_data_t initRequestData(ServerInfo const &serverInfo, HttpRequest const &request, std::string const &requestType);
	public:
		HttpMethod();
		HttpMethod(HttpMethod const &httpMethod);
		virtual ~HttpMethod();

		virtual HttpResponse execute(ServerInfo const &info, HttpRequest &request) = 0;

		virtual std::string getName() = 0;
		virtual HttpMethod *clone() = 0;

		HttpMethod &operator=(HttpMethod const &rhs);
};

# include "HttpRequest.hpp"

#endif
