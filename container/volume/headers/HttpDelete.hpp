#ifndef HTTPDELETE_HPP
# define HTTPDELETE_HPP

# include "HttpMethod.hpp"

class HttpDelete : public HttpMethod {
	private:

	public:
		HttpDelete();
		HttpDelete(HttpDelete const &httpDelete);
		~HttpDelete();

		HttpResponse execute(ServerInfo const &serverInfo, HttpRequest &request);
		HttpMethod *clone();

		std::string getName();

		HttpDelete &operator=(HttpDelete const &rhs);
};

#endif
