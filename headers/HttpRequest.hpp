#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <vector>
# include <string>

# include "HttpMessage.hpp"
# include "HttpMethod.hpp"
# include "FileBody.hpp"
# include "ServerInfo.hpp"

class HttpRequest : public HttpMessage {
	private:
		HttpMethod *_method;
		std::string _path;
	public:
		HttpRequest();
		HttpRequest(HttpRequest const &httpRequest);
		~HttpRequest();

		const std::string build() const;
		void execute(ServerInfo const &serverInfo);

		void setMethod(HttpMethod *method);
		void setPath(std::string path);

		HttpMethod *getMethod() const;
		std::string getPath() const;

		HttpRequest &operator=(HttpRequest const &rhs);
};

#endif
