#include "RequestParser.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

RequestParser::RequestParser() : _headersReceived(false) {}
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
		headers.erase(0, endLinePos + 2);																			 // +2 skips CRLF
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

void RequestParser::parseBody(std::string messageBody) {
	MessageBody &body = _httpRequest.getMessageBody();
	if (messageBody.empty())
		return;
	body.append(messageBody);
	if (body.getSize() >= ws::stoi(_httpRequest.getHeader("Content-Length")))
		_requestParsed = true;
}

int RequestParser::readChunked(std::string body) {
	std::cout << "body: " << std::endl << body << std::endl << "end body------" << std::endl;
	if (_hex.empty()) {
		_hex = body.substr(0, body.find("\r\n"));
		_sizeRead = 0;
		body.erase(0, body.find("\r\n") + 2);
	} else {
		
	}
	_requestParsed = true;
	return 1;
}

// ############## PUBLIC ##############

bool RequestParser::parseRequest(std::string request) {
	if (!_headersReceived) {
		if (request.find("\r\n\r\n") != std::string::npos) {
			std::string str = _inReceive.str() + request;
			_inReceive.str(""); // clears the stream
			parseFirstLine(str.substr(0, str.find("\r\n")));
			parseHeaders(str.substr(str.find("\r\n") + 2));
			request = _inReceive.str(); // contains the possible body following headers
			_inReceive.str("");
		} else
			_inReceive << request;
	}
	if (!_headersReceived)
		return false;

	if (_httpRequest.getHeader("Transfer-Encoding") == "chunked") {
		if (_hex.empty() && request.find("\r\n") == std::string::npos) {
			std::cout << "request : " << request << std::endl;
			_inReceive << request;
			return true;
		}
		if (_inReceive.rdbuf()->in_avail() != 0) {
			request += _inReceive.str();
			_inReceive.str("");
		}
		readChunked(request);
	} else
		parseBody(request);

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
		_hex = rhs._hex;
	}
	return *this;
}