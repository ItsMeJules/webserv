#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP

# include <vector>
# include <string>

# include "utils.hpp"
# include "HttpRequest.hpp"
# include "ChunkedBody.hpp"
# include "FileBody.hpp"
# include "RegularBody.hpp"

class RequestParser {
	private:
		std::stringstream _inReceive;
		bool _headersReceived;

		HttpRequest _httpRequest;

		void parseFirstLine(std::string firstLine);
		bool parseHeaders(std::string headers);

		std::string emptyAndClearStream();
        IMessageBody *getAccordingBodyType();

	public:
		RequestParser();
		RequestParser(RequestParser const &request);
		~RequestParser();

		bool parseRequest(std::string request);

		HttpRequest &getHttpRequest();

		RequestParser &operator=(RequestParser const &rhs);
};

    #endif