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

		int _errorCode;

		HttpRequest _httpRequest;

		bool parseFirstLine(std::string firstLine);
		bool parseHeaders(std::string headers);

		std::string emptyAndClearStream();
        AMessageBody *getAccordingBodyType();

		std::string _query;
	public:
		RequestParser();
		RequestParser(RequestParser const &request);
		~RequestParser();

		bool parseRequest(char *request, int &byteCount, int const &maxBodySize);
        void clear();

		HttpRequest &getHttpRequest();
		const bool &isRequestParsed() const;
		const int &getErrorCode() const;

		RequestParser &operator=(RequestParser const &rhs);
};

    #endif
