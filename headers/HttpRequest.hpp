#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <vector>
# include <string>

# include "Message.hpp"

class HttpRequest : public Message {
	private:
		std::string _method;
		std::string _path;
		std::vector<std::string> _data;
	public:
		HttpRequest();
		HttpRequest(HttpRequest const &httpRequest);
		~HttpRequest();

		std::string build();

		void setMethod(std::string method);
		void setPath(std::string path);

		std::string getMethod() const;
		std::string getPath() const;
		std::vector<std::string>	getData() const;

		HttpRequest &operator=(HttpRequest const &rhs);
};

#endif
