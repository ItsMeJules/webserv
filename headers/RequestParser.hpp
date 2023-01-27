#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP

# include <vector>
# include <string>

# include "utils.hpp"
# include "HttpRequest.hpp"

# include "AMessageBody.hpp"
# include "FormDataBody.hpp"
# include "DefaultBody.hpp"

# include "ADataDecoder.hpp"
# include "ChunkedDataDecoder.hpp"
# include "DefaultDataDecoder.hpp"


class RequestParser {
	private:
		std::stringstream _inReceive;
		bool _headersReceived;
		bool _requestParsed;

		HttpRequest _httpRequest;

		void parseFirstLine(std::string firstLine);

		std::string emptyAndClearStream();
        AMessageBody *getAccordingBodyType();
	public:
		bool parseHeaders(std::string headers);
		RequestParser();
		RequestParser(RequestParser const &request);
		~RequestParser();

		const bool parseRequest(char *request, int &byteCount);
        void clear();

		HttpRequest &getHttpRequest();
		const bool isRequestParsed() const;

		RequestParser &operator=(RequestParser const &rhs);
		// void	returnParse(std::string headers);

};

    #endif
