#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <vector>
# include <string>

# include "Message.hpp"
# include "FileBody.hpp"
# include "ServerInfo.hpp"

class HttpRequest : public Message {
	private:
		std::string _method;
		std::string _path;
	public:
		HttpRequest();
		HttpRequest(HttpRequest const &httpRequest);
		~HttpRequest();

		std::string build();
		void execute(ServerInfo const &serverInfo);

		void setMethod(std::string method);
		void setPath(std::string path);

		std::string getMethod() const;
		std::string getPath() const;

		HttpRequest &operator=(HttpRequest const &rhs);
};

#endif
