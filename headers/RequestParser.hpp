#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP

# include <vector>
# include <string>

# include "utils.hpp"

# define HEX_VALUES "0123456789abcdef"

class HttpRequest;

class RequestParser {
	private:
		std::stringstream _inReceive;
		bool _headersReceived;

		HttpRequest &_request;

		std::string hex;

		void parseFirstLine(std::string firstLine);
		bool parseHeaders(std::string headers);
		void parseBody(std::string messageBody);
	public:
		RequestParser();
		RequestParser(HttpRequest &request);
		RequestParser(RequestParser const &requestParser);
		~RequestParser();

		bool parseRequest(std::string request);
		int readChunked(std::string body);

		RequestParser &operator=(RequestParser const &rhs);
};

# include "HttpRequest.hpp"

#endif