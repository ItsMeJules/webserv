#ifndef HTTPMETHOD_HPP
# define HTTPMETHOD_HPP

# include <string>

# include "ServerInfo.hpp"
# include "HttpResponse.hpp"

class HttpRequest;

class HttpMethod {
	private:
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