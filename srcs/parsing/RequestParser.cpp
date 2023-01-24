#include "RequestParser.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

RequestParser::RequestParser() : _headersReceived(false), _requestParsed(false) {}
RequestParser::RequestParser(RequestParser const &request) { *this = request; }
RequestParser::~RequestParser() {}

// ############## PRIVATE ##############

void RequestParser::parseFirstLine(std::string firstLine) {
	size_t pos = 0;
	while ((pos = firstLine.find(' ')) != std::string::npos) {
		if (_httpRequest.getMethod() == NULL)
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
		size_t separatorPos = headers.find(':');
		_httpRequest.addHeader(headers.substr(0, separatorPos), headers.substr(separatorPos + 2, endLinePos - separatorPos - 2)); // skips ": " and stops before CRLF
		headers.erase(0, endLinePos + 2); // +2 skips CRLF
		if (headers.rfind("\r\n", 0) != std::string::npos) { // does the string start with \r\n
			_headersReceived = true;
			ws::log(ws::LOG_LVL_ALL, "[REQUEST PARSER] -", "request metadata was parsed");
			if (headers.size() == 2) // theres no body after the headers
				return false;
			_inReceive << headers.substr(2);
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

IMessageBody *RequestParser::getAccordingBodyType() {
	IMessageBody *body;

    if (_httpRequest.headersHasKey("Transfer-Encoding") && _httpRequest.getHeader("Transfer-Encoding") == "chunked")
			body = new ChunkedBody();
    else {
		if (_httpRequest.headersHasKey("Content-Length"))
			body = new RegularBody(ws::stoi(_httpRequest.getHeader("Content-Length")));
		else
			body = new RegularBody();
	}
	if (_httpRequest.headersHasKey("Content-Type") && _httpRequest.getHeader("Content-Type").rfind("multipart/form-data;", 0) != std::string::npos)
			return new FileBody(body, _httpRequest.getHeader("Content-Type"));
	return body;
}

// ############## PUBLIC ##############

const bool RequestParser::parseRequest(std::string request, int const &byteCount) {
	if (!_headersReceived) {
		size_t endHeaders = request.find("\r\n\r\n");
		if (endHeaders != std::string::npos) {
			ws::log(ws::LOG_LVL_DEBUG, "[REQUEST PARSER] -", "about to parse " + ws::itos(endHeaders + 4) + " chars from headers.");
			std::string str = emptyAndClearStream() + request;
			parseFirstLine(str.substr(0, str.find("\r\n")));
			if (!parseHeaders(str.substr(str.find("\r\n") + 2))) // there's no body
				_requestParsed = true;
			else {
				ws::log(ws::LOG_LVL_DEBUG, "[REQUEST PARSER] -", "about to parse " + ws::itos(byteCount - endHeaders - 4) + " chars from body received with headers.");
				parseRequest(emptyAndClearStream(), byteCount - endHeaders - 4); // removes to byteCount header size.
			}
		} else {
			ws::log(ws::LOG_LVL_DEBUG, "[REQUEST PARSER] -", "data stored in stringstream");
			_inReceive << request;
		}
	} else {
        if (_httpRequest.getMessageBody() == NULL)
            _httpRequest.setMessageBody(getAccordingBodyType());
		int ret = _httpRequest.getMessageBody()->parse(emptyAndClearStream() + request, _inReceive, byteCount);
		if (ret < 0)
			return false;
        _requestParsed = ret == 1;
		if (_requestParsed) {
			ws::log(ws::LOG_LVL_ALL, "[REQUEST PARSER] -", "request was fully parsed");
			ws::log(ws::LOG_LVL_DEBUG, "[REQUEST PARSER] -", "contents:\n----------\n" + _httpRequest.build() + "\n----------");
		}
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

const bool RequestParser::isRequestParsed() const {
	return _requestParsed;
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