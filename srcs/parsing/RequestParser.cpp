#include "RequestParser.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

RequestParser::RequestParser() : _headersReceived(false), _requestParsed(false) {}
RequestParser::RequestParser(RequestParser const &request) { *this = request; }
RequestParser::~RequestParser() {}

// ############## PRIVATE ##############

bool RequestParser::parseFirstLine(std::string firstLine) {
	size_t pos;

	while ((pos = firstLine.find(' ')) != std::string::npos) {
		if (_httpRequest.getMethod() == NULL) {
			if (!_httpRequest.setMethod(firstLine.substr(0, pos)))
				return false;
		} else if (_httpRequest.getPath().empty())
			_httpRequest.setPath(firstLine.substr(0, pos));
		firstLine.erase(0, pos + 1); // coupe jusqu'a apres l'espace
	}
	_httpRequest.setHttpVersion(firstLine);
	return true;
}

bool RequestParser::parseHeaders(std::string headers) {
	size_t endLinePos;
	while ((endLinePos = headers.find("\r\n")) != std::string::npos) {
		size_t separatorPos = headers.find(':');
		_httpRequest.addHeader(headers.substr(0, separatorPos), headers.substr(separatorPos + 2, endLinePos - separatorPos - 2)); // skips ": " and stops before CRLF
		headers.erase(0, endLinePos + 2); // +2 skips CRLF
		if (headers.rfind("\r\n", 0) != std::string::npos) { // does the string start with \r\n
			_headersReceived = true;
			ws::log(ws::LOG_LVL_ALL, "[REQUEST PARSER] -", "request metadata was parsed");
			if (headers.size() == 2) // theres no body after the headers
				return false;
			break;
		}
	}
	return true;
}

std::string RequestParser::emptyAndClearStream() {
	std::string str = _inReceive.str();
	_inReceive.str("");
	return str;
}

AMessageBody *RequestParser::getAccordingBodyType() {
	ADataDecoder *decoder;

    if (_httpRequest.headersHasKey("Transfer-Encoding") && _httpRequest.getHeader("Transfer-Encoding") == "chunked")
			decoder = new ChunkedDataDecoder();
    else {
		if (_httpRequest.headersHasKey("Content-Length"))
			decoder = new DefaultDataDecoder(ws::stoi(_httpRequest.getHeader("Content-Length")));
		else
			decoder = new DefaultDataDecoder();
	}
	if (_httpRequest.headersHasKey("Content-Type") && _httpRequest.getHeader("Content-Type").rfind("multipart/form-data;", 0) != std::string::npos)
			return new FormDataBody(decoder, _httpRequest.getHeader("Content-Type"));
	return new DefaultBody(decoder);
}

// ############## PUBLIC ##############

bool RequestParser::parseRequest(char *request, int &byteCount, int const &maxBodySize) {
	if (!_headersReceived) {
		std::string recv = _inReceive.str();
		std::string wholeRequest = recv + std::string(request, byteCount);
		size_t endHeaders = wholeRequest.find("\r\n\r\n");

		if (endHeaders != std::string::npos) {
			_inReceive.str("");
			ws::log(ws::LOG_LVL_DEBUG, "[REQUEST PARSER] -", "about to parse " + ws::itos(endHeaders) + " chars from headers.");

			if (!parseFirstLine(wholeRequest.substr(0, wholeRequest.find("\r\n"))))
				return false;
			if (!parseHeaders(wholeRequest.substr(wholeRequest.find("\r\n") + 2))) // there's no body
				_requestParsed = true;
			else {
				int bodySize = wholeRequest.size() - (endHeaders + 4);
				ws::log(ws::LOG_LVL_DEBUG, "[REQUEST PARSER] -", "about to parse " + ws::itos(bodySize) + " chars from body received with headers.");

				std::vector<char> vector(wholeRequest.data() + (wholeRequest.size() - bodySize), wholeRequest.data() + wholeRequest.size());
				parseRequest(vector.data(), bodySize, maxBodySize); // removes to byteCount header size.
			}
		} else {
			ws::log(ws::LOG_LVL_DEBUG, "[REQUEST PARSER] -", "data stored in stringstream");
			_inReceive << request;
		}
	} else {
		if (_httpRequest.headersHasKey("Content-Length") && ws::stoi(_httpRequest.getHeader("Content-Length")) > maxBodySize) {
				_errorCode = 413;
				return false;
		}

        if (_httpRequest.getMessageBody() == NULL)
            _httpRequest.setMessageBody(getAccordingBodyType());

		int ret = _httpRequest.getMessageBody()->parse(request, byteCount);
		if (ret < 0) {
			_errorCode = 400;
			return false;
		}

        _requestParsed = ret == 1;
    }
	if (_requestParsed) {
		ws::log(ws::LOG_LVL_ALL, "[REQUEST PARSER] -", "request was fully parsed");
		ws::log(ws::LOG_LVL_DEBUG, "[REQUEST PARSER] -", "contents:\n----------\n" + _httpRequest.build() + "\n----------");
	}
	return true;
}

void RequestParser::clear() {
    HttpRequest empty;
    _headersReceived = false;
    _httpRequest = empty;
	_requestParsed = false;
    _inReceive.str("");
}

// ############## GETTERS / SETTERS ##############

HttpRequest &RequestParser::getHttpRequest() {
	return _httpRequest;
}

const bool &RequestParser::isRequestParsed() const {
	return _requestParsed;
}

const int &RequestParser::getErrorCode() const {
	return _errorCode;
}

// ############## OPERATORS ##############

RequestParser &RequestParser::operator=(RequestParser const &rhs) {
	if (this != &rhs) {
		_inReceive << rhs._inReceive.str();
		_headersReceived = rhs._headersReceived;
		_httpRequest = rhs._httpRequest;
		_requestParsed = rhs._requestParsed;
	}
	return *this;
}
