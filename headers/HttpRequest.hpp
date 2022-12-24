#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <vector>
# include <string>

# include "Message.hpp"
# include "RequestParser.hpp"

class HttpRequest : public Message {
	private:
		std::string _method;
		std::string _path;

		RequestParser _parser;
	public:
		HttpRequest();
		HttpRequest(HttpRequest const &httpRequest);
		~HttpRequest();

		std::string build();

		void setMethod(std::string method);
		void setPath(std::string path);

		std::string getMethod() const;
		std::string getPath() const;
		RequestParser &getParser();

		HttpRequest &operator=(HttpRequest const &rhs);
};

#endif
