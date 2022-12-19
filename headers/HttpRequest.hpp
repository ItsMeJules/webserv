#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <vector>
# include <string>

# include "Message.hpp"

class HttpRequest : public Message {
	private:
		std::vector<std::string> _inReceive;

		std::string _method;
		std::string _path;

		std::string concatenateDataReceived(std::string request = NULL);
	protected:
		bool _headersReceived;

		void parseFirstLine(std::string firstLine);
		bool parseHeaders(std::string headers);
		void parseBody(std::string messageBody);
		void readChunked(std::string body);
	public:
		HttpRequest();
		HttpRequest(HttpRequest const &httpRequest);
		~HttpRequest();

		std::string build();
		void parse(std::string request);

		void setMethod(std::string method);
		void setPath(std::string path);

		std::string getMethod() const;
		std::string getPath() const;

		HttpRequest &operator=(HttpRequest const &rhs);
};

#endif
