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
		bool _requestParsed;

		HttpRequest _httpRequest;

		void parseFirstLine(std::string firstLine);

		std::string emptyAndClearStream();
        IMessageBody *getAccordingBodyType();

	public:
		bool parseHeaders(std::string headers);
		RequestParser();
		RequestParser(RequestParser const &request);
		~RequestParser();

		void parseRequest(std::string request);
        void clear();

		HttpRequest &getHttpRequest();
		const bool isRequestParsed() const;

		RequestParser &operator=(RequestParser const &rhs);
		// void	returnParse(std::string headers);

};

    #endif
