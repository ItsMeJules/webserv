#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <string>

# include "Message.hpp"

class HttpRequest : public Message {
	private:
		std::string _method;
		std::string _path;
		std::string _httpVersion;
	protected:
		void parseFirstLine(std::string firstLine);
		void parseHeaders(std::string headers);
		void parseBody(std::string messageBody);
	public:
		HttpRequest();
		HttpRequest(std::string method, std::string path, std::string httpVersion);
		HttpRequest(HttpRequest const &httpRequest);
		~HttpRequest();

		std::string build() const;
		void parse(std::string &request);

		void setMethod(std::string method);
		void setPath(std::string path);
		void setHttpVersion(std::string httpVersion);

		std::string getMethod() const;
		std::string getPath() const;
		std::string getHttpVersion() const;

		HttpRequest &operator=(HttpRequest const &rhs);
};

#endif
