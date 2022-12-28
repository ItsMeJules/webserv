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

std::string RequestParser::concatenateDataReceived(std::string request) {
	std::string str;
	for (std::vector<std::string>::iterator it = _inReceive.begin(); it != _inReceive.end(); it++)
		str += *it;
	return request.empty() ? str : str + request;
}

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
			_inReceive.push_back(headers.substr(2));
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
		if (request.find("\r\n\r\n") == std::string::npos) {
			_inReceive.push_back(request);
		} else {
			std::string str = concatenateDataReceived(request);
			parseFirstLine(str.substr(0, str.find("\r\n")));
			parseHeaders(str.substr(str.find("\r\n") + 2));
			request = *(_inReceive.end() - 1);
			_inReceive.clear();
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

void RequestParser::readChunked(std::string body) {
	_request.setRequestReceived(true);
}

// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

RequestParser &RequestParser::operator=(RequestParser const &rhs) {
	if (this != &rhs) {
		_inReceive = rhs._inReceive;
		_headersReceived = rhs._headersReceived;
		_request = rhs._request;
	}
	return *this;
}
