#include "RequestParser.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

RequestParser::RequestParser() : _headersReceived(false), _requestParsed(false) {}
RequestParser::RequestParser(RequestParser const &request) { *this = request; }
RequestParser::~RequestParser() {}

// ############## PRIVATE ##############

void RequestParser::parseFirstLine(std::string firstLine) {
	size_t pos;
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
	if (true)
		std::cout << "HEADER IS OKAY" << std::endl;
	else
		std::cout << "HEADER IS NOT OKAY" << std::endl;
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

const bool RequestParser::parseRequest(char *request, int &byteCount) {
	if (!_headersReceived) {
		std::string requestStr = request;
		size_t endHeaders = requestStr.find("\r\n\r\n");
		if (endHeaders != std::string::npos) {
			ws::log(ws::LOG_LVL_DEBUG, "[REQUEST PARSER] -", "about to parse " + ws::itos(endHeaders + 4) + " chars from headers.");
			std::string str = emptyAndClearStream() + requestStr;
			parseFirstLine(str.substr(0, str.find("\r\n")));
			if (!parseHeaders(str.substr(str.find("\r\n") + 2))) // there's no body
				_requestParsed = true;
			else {
				int bodySize = byteCount - endHeaders - 4;
				ws::log(ws::LOG_LVL_DEBUG, "[REQUEST PARSER] -", "about to parse " + ws::itos(bodySize) + " chars from body received with headers.");

				std::vector<char> vector(request + (byteCount - bodySize), request + byteCount + 1);
				parseRequest(vector.data(), bodySize); // removes to byteCount header size.
			}
		} else {
			ws::log(ws::LOG_LVL_DEBUG, "[REQUEST PARSER] -", "data stored in stringstream");
			_inReceive << request;
		}
	} else {
        if (_httpRequest.getMessageBody() == NULL)
            _httpRequest.setMessageBody(getAccordingBodyType());
		int ret = _httpRequest.getMessageBody()->parse(request, byteCount);
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
