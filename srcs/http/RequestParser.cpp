#include "RequestParser.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

RequestParser::RequestParser() : _headersReceived(false), _hexSize(-1) {}
RequestParser::RequestParser(RequestParser const &request) { *this = request; }
RequestParser::~RequestParser() {}

// ############## PRIVATE ##############

void RequestParser::parseFirstLine(std::string firstLine) {
	size_t pos = 0;
	while ((pos = firstLine.find(' ')) != std::string::npos) {
		if (_httpRequest.getMethod().empty())
			_httpRequest.setMethod(firstLine.substr(0, pos));
		else if (_httpRequest.getPath().empty())
			_httpRequest.setPath(firstLine.substr(0, pos));
		firstLine.erase(0, pos + 1); // coupe jusqu'a apres l'espace
	}
	_httpRequest.setHttpVersion(firstLine);
}

bool RequestParser::parseHeaders(std::string headers) {
	size_t endLinePos = 0;
	while ((endLinePos = headers.find("\r\n")) != std::string::npos) {
		size_t separator = headers.find(':');
		_httpRequest.addHeader(headers.substr(0, separator), headers.substr(separator + 2, endLinePos - separator - 2)); // skips ": " and stops before CRLF
		headers.erase(0, endLinePos + 2); // +2 skips CRLF
		if (headers.rfind("\r\n", 0) != std::string::npos) { // does the string start with \r\n
			_headersReceived = true;
			if (headers.size() == 2) // theres no body after the headers
				return false;
			_inReceive << headers.substr(2);
			break;
		}
	}
	return true;
}

int RequestParser::parseBody(std::string messageBody) {
	MessageBody &body = _httpRequest.getMessageBody();
    int contentLength = ws::stoi(_httpRequest.getHeader("Content-Length"));
    if (body.getSize() < contentLength) {
        size_t endPos = messageBody.find(("\r\n\r\n"));
        // if the size of the actual messsageBody - 4 (\r\n\r\n) + what's already parsed is lower than content length error
        if (endPos != std::string::npos && body.getSize() + messageBody.size() - 4 < contentLength) {
            std::cerr << "the size of the body (" << body.getSize() << ") differs from the content length (" << contentLength << ")." << std::endl;
            return -2;
        }
        body.append(endPos == std::string::npos ? messageBody.substr(0, endPos) : messageBody);
    }
    if (body.getSize() > contentLength) {
        std::cerr << "the size of the body (" << body.getSize() << ") differs from the content length (" << contentLength << ")." << std::endl;
        return -1;
    }
    _requestParsed = true;
    return 1;
}

int RequestParser::readChunked(std::string body) {
    body = emptyAndClearStream() + body;
    size_t pos = body.find("\r\n");
    if (_hexSize == -1) {
        if (pos == std::string::npos)
            _inReceive << body;
        else {
            std::string hexStr = body.substr(0, pos);
            if (!ws::string_in_range(HEX_VALUES, hexStr)) {
                std::cerr << "error while reading chunk size. \"" << hexStr << "\" isn't a valid hex value." << std::endl;
                return -1;
            }
            _hexSize = ws::hextoi(hexStr);
            if (_hexSize != 0 && !body.erase(0, pos + 2).empty()) // deletes up to \r\n
                readChunked(body);
        }
    } else {
        if (pos == std::string::npos)
            _inReceive << body;
        else {
            pos = body.find("\r\n");
            std::string chunkContent = body.substr(0, pos);
            if (chunkContent.size() > _hexSize) {
                std::cerr << "error while reading chunk : \"" << chunkContent << "\" the chunk size is bigger than the given size (" << _hexSize << ")." << std::endl;
                return -2;
            }
            _httpRequest.getMessageBody().append(chunkContent);
            _inReceive << body.erase(0, pos + 2);
            _hexSize = -1;
            // just in case we received the 0 and parseRequest won't be called again bc recv read everything.
            if (body.find("\r\n\r\n") != std::string::npos)
                readChunked("");
        }
    }
	return _hexSize == 0;
}

std::string RequestParser::emptyAndClearStream() {
	std::string str = _inReceive.str();
	_inReceive.str("");
	return str;
}

// ############## PUBLIC ##############

bool RequestParser::parseRequest(std::string request) {
	if (!_headersReceived) {
		if (request.find("\r\n\r\n") != std::string::npos) {
			std::string str = emptyAndClearStream() + request;
			parseFirstLine(str.substr(0, str.find("\r\n")));
			parseHeaders(str.substr(str.find("\r\n") + 2));
			request = emptyAndClearStream(); // contains the possible body following headers
			if (!request.empty())
				parseRequest(request);
		} else
			_inReceive << request;
	} else {
		if (_httpRequest.getHeader("Transfer-Encoding") == "chunked")
			_requestParsed = readChunked(request);
		else
			parseBody(request);
	}
	return true;
}

// ############## GETTERS / SETTERS ##############

bool RequestParser::isRequestParsed() const {
	return _requestParsed;
}

HttpRequest &RequestParser::getHttpRequest() {
	return _httpRequest;
}

// ############## OPERATORS ##############

RequestParser &RequestParser::operator=(RequestParser const &rhs) {
	if (this != &rhs) {
		_inReceive << rhs._inReceive.str();
		_headersReceived = rhs._headersReceived;
		_requestParsed = rhs._requestParsed;
		_httpRequest = rhs._httpRequest;
		_hexSize = rhs._hexSize;
	}
	return *this;
}