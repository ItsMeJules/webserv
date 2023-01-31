#ifndef HTTPGET_HPP
# define HTTPGET_HPP

# include "HttpMethod.hpp"
# include "DefaultBody.hpp"

class Cgi;

class HttpGet : public HttpMethod {
	private:
		std::string		_newIndex;

	public:
		HttpGet();
		HttpGet(HttpGet const &httpGet);
		~HttpGet();

		HttpResponse execute(ServerInfo const &info, HttpRequest &request);
		HttpMethod *clone();

		std::string getName();
		// bool	isCgi();
		HttpGet &operator=(HttpGet const &rhs);
};

#endif

