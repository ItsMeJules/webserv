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
		bool parseHeaders(std::string headers);

		std::string emptyAndClearStream();
        AMessageBody *getAccordingBodyType();

		std::string _query;
	public:
		RequestParser();
		RequestParser(RequestParser const &request);
		~RequestParser();

		bool parseRequest(char *request, int &byteCount);
        void clear();

		std::string	findQuery(void);
		std::string	getQuery() const;

		HttpRequest &getHttpRequest();
		const bool &isRequestParsed() const;

		RequestParser &operator=(RequestParser const &rhs);
};

    #endif
