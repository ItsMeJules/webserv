#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <vector>
# include <string>

# include "HttpMessage.hpp"
# include "HttpMethod.hpp"
# include "HttpGet.hpp"
# include "HttpPost.hpp"
# include "HttpDelete.hpp"
# include "HttpResponse.hpp"
# include "ServerInfo.hpp"

class HttpRequest : public HttpMessage {
	private:
		HttpMethod *_method;
		std::string _path;

		std::vector<std::string> _data;
		bool	_isCgi;

	public:
		static std::map<std::string, HttpMethod*> methods;

	public:
		HttpRequest();
		HttpRequest(HttpRequest const &httpRequest);
		~HttpRequest();

		const std::string build() const;
		HttpResponse execute(ServerInfo const &serverInfo);

		bool setMethod(std::string method);
		void setPath(std::string path);

		HttpMethod *getMethod() const;
		std::string getPath() const;
		std::vector<std::string>	getData() const;

		bool IsCgi();

		HttpRequest &operator=(HttpRequest const &rhs);

		static std::map<std::string, HttpMethod*> initMethods();
		static void clearMethods();
};

#endif
