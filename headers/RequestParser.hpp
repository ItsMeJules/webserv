#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP

# include <vector>
# include <string>

# include "utils.hpp"
# include "HttpRequest.hpp"

# define HEX_VALUES "0123456789abcdef"

class RequestParser {
	private:
		std::stringstream _inReceive;
        std::string _boundary;
		bool _headersReceived;

		HttpRequest _httpRequest;

		int _hexSize;

		void parseFirstLine(std::string firstLine);
		bool parseHeaders(std::string headers);
		int parseBody(std::string messageBody);
        int readChunked(std::string body);
        int readFile(std::string body);

		std::string emptyAndClearStream();
	public:
		RequestParser();
		RequestParser(RequestParser const &request);
		~RequestParser();

		bool parseRequest(std::string request);

		HttpRequest &getHttpRequest();

		RequestParser &operator=(RequestParser const &rhs);
};

    #endif