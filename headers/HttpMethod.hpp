#ifndef HTTPMETHOD_HPP
# define HTTPMETHOD_HPP

# include <string>

# include "ServerInfo.hpp"
# include "HttpResponse.hpp"

class HttpMethod {
	private:
	public:
		HttpMethod();
		HttpMethod(HttpMethod const &httpMethod);
		virtual ~HttpMethod();

		virtual HttpResponse execute() = 0;

		virtual std::string getName() = 0;

		HttpMethod &operator=(HttpMethod const &rhs);
};

#endif