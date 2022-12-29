#include "RequestParser.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

RequestParser::RequestParser(HttpRequest &request) : _request(request) {}
RequestParser::RequestParser(RequestParser const &requestParser)
	: _request(requestParser._request)
{
	*this = requestParser;
}
RequestParser::~RequestParser() {}

// ############## PRIVATE ##############

void RequestParser::parseFirstLine(std::string firstLine) {
	size_t pos = 0;
	while ((pos = firstLine.find(' ')) != std::string::npos) {
		if (_request.getMethod().empty())
			_request.setMethod(firstLine.substr(0, pos));
		else if (_request.getPath().empty())
			_request.setPath(firstLine.substr(0, pos));
		firstLine.erase(0, pos + 1); //coupe jusqu'a apres l'espace
	}
	_request.setHttpVersion(firstLine);
}

bool RequestParser::parseHeaders(std::string headers) {
	size_t endLinePos = 0;
	while ((endLinePos = headers.find("\r\n")) != std::string::npos) {
		size_t separator = headers.find(':');
		_request.addHeader(headers.substr(0, separator), headers.substr(separator + 2, endLinePos - separator - 2)); // skips ": " and stops before CRLF
		headers.erase(0, endLinePos + 2); // +2 skips CRLF
		if (headers.rfind("\r\n", 0) != std::string::npos) { // does the string start with \r\n
			_headersReceived = true;
			if (headers.size() == 2) //theres no body after the headers
				return false;
			_inReceive << headers.substr(2);
			break ;
		}
	}
	return true;
}

void RequestParser::parseBody(std::string messageBody) {
	MessageBody &body = _request.getMessageBody();
	if (messageBody.empty())
		return ;
	body.append(messageBody);
	if (messageBody.getSize() >= ws::stoi(_request.getHeader("Content-Length")))
		_request.setRequestReceived(true);
}

// ############## PUBLIC ##############

bool RequestParser::parseRequest(std::string request) {
	if (!_headersReceived) {
		_inReceive << request;
		if (request.find("\r\n\r\n") != std::string::npos) {
			std::string str = _inReceive.str();
			_inReceive.str("");
			parseFirstLine(str.substr(0, str.find("\r\n")));
			parseHeaders(str.substr(str.find("\r\n") + 2));
			request = _inReceive.str();
		}
	}
	if (_headersReceived) {
		if (_request.getHeader("Transfer-Encoding") == "chunked")
			readChunked(request);
		else
			parseBody(request);
	}
	return true;
}

int RequestParser::readChunked(std::string body) {
	for (size_t end = body.find("\r\n"); end != std::string::npos;) {
		if (hex.empty()) {
			hex = body.substr()
			for (int i = 0; i < body.size(); i++) {
				if ((body[i] == '\r' || body[i] == '\n') && !hex.empty())
					break ;
				if (!ws::char_in_string(HEX_VALUES, body[i]))
					return -1;
				hex += body[i];
			}
		}
	}
	
	for (int i = 0; i < body.size(); i++) {
		if (hex.empty()) {
			if ((body[i] == '\r' || body[i] == '\n') && !hex.empty())
				break ;
			if (!ws::char_in_string(HEX_VALUES, body[i]))
				return -1;
			hex += body[i];
		} else {
			hex.clear();
		}
	}
	MessageBody &body = _request.getMessageBody();
	int pos = body.find("\r\n");
	_request.setRequestReceived(true);
}

// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

RequestParser &RequestParser::operator=(RequestParser const &rhs) {
	if (this != &rhs) {
		_inReceive << rhs._inReceive.str();
		_headersReceived = rhs._headersReceived;
		_request = rhs._request;
	}
	return *this;
}
