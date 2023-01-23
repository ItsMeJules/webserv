#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP

# include <vector>
# include <string>

# include "utils.hpp"
# include "HttpRequest.hpp"
# include "ChunkedBody.hpp"
# include "RegularBody.hpp"

class RequestParser {
	private:
		std::stringstream _inReceive;
		bool _headersReceived;
		bool _requestParsed;

		HttpRequest _httpRequest;

		void parseFirstLine(std::string firstLine);
		bool parseHeaders(std::string headers);

		std::string emptyAndClearStream();
        IMessageBody *getAccordingBodyType();

	public:
		RequestParser();
		RequestParser(RequestParser const &request);
		~RequestParser();

		const bool parseRequest(std::string request);
        void clear();

		HttpRequest &getHttpRequest();
		const bool isRequestParsed() const;

		RequestParser &operator=(RequestParser const &rhs);
};

    #endif