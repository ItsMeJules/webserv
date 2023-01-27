#ifndef HTTPDELETE_HPP
# define HTTPDELETE_HPP

# include "HttpMethod.hpp"

class HttpDelete : public HttpMethod {
	private:
		std::string _path;

	public:
		HttpDelete();
		HttpDelete(HttpDelete const &httpDelete);
		~HttpDelete();

		HttpResponse execute(ServerInfo const &info, HttpRequest &request);
		HttpMethod *clone();

		std::string getName();
		std::string	getPath() const;

		HttpDelete &operator=(HttpDelete const &rhs);
};

#endif

