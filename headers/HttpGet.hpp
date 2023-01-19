#ifndef HTTPGET_HPP
# define HTTPGET_HPP

# include "HttpMethod.hpp"

class HttpGet : public HttpMethod {
	private:
		
	public:
		HttpGet();
		HttpGet(HttpGet const &httpGet);
		~HttpGet();

		HttpResponse execute();

		std::string getName();

		HttpGet &operator=(HttpGet const &rhs);
};

#endif