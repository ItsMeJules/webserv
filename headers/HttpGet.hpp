#ifndef HTTPGET_HPP
# define HTTPGET_HPP

# include "HttpMethod.hpp"
# include "DefaultBody.hpp"

class HttpGet : public HttpMethod {
	private:
	public:
		HttpGet();
		HttpGet(HttpGet const &httpGet);
		~HttpGet();

		HttpResponse execute(ServerInfo const &info, HttpRequest &request);
		HttpMethod *clone();

		std::string getName();

		HttpGet &operator=(HttpGet const &rhs);
};

#endif

