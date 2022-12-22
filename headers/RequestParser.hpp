#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP

# include <vector>
# include <string>

# include "HttpRequest.hpp"

class HttpRequest;

class RequestParser {
	private:
		std::vector<std::string> _inReceive;
		bool _headersReceived;

		HttpRequest &_request;

		void parseFirstLine(std::string firstLine);
		bool parseHeaders(std::string headers);
		void parseBody(std::string messageBody);
		std::string concatenateDataReceived(std::string request = "");
	public:
		RequestParser(HttpRequest &request);
		RequestParser(RequestParser const &requestParser);
		~RequestParser();

		bool parseRequest(std::string request);
		void readChunked(std::string body);

		RequestParser &operator=(RequestParser const &rhs);
};

#endif