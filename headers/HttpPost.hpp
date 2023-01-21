#ifndef HTTPPOST_HPP
# define HTTPPOST_HPP

# include "HttpMethod.hpp"
# include "FileBody.hpp"

class HttpPost : public HttpMethod {
	private:
		
	public:
		HttpPost();
		HttpPost(HttpPost const &httpPost);
		~HttpPost();

		HttpResponse execute(ServerInfo const &info, HttpRequest &request);
		HttpMethod *clone();

		std::string getName();
		
		HttpPost &operator=(HttpPost const &rhs);
};

#endif