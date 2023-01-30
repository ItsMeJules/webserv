#ifndef HTTPGET_HPP
# define HTTPGET_HPP

# include <fstream>
# include <sys/stat.h>

# include "HttpMethod.hpp"
# include "DefaultBody.hpp"
# include "CGI.hpp"

class HttpGet : public HttpMethod {
	private:
		bool	_isCgi;
	public:
		HttpGet();
		HttpGet(HttpGet const &httpGet);
		~HttpGet();

		HttpResponse execute(ServerInfo const &info, HttpRequest &request);
		HttpMethod *clone();

		std::string getName();
		bool	isCgi();
		HttpGet &operator=(HttpGet const &rhs);
};

#endif
