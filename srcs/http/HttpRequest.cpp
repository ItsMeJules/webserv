#include "HttpRequest.hpp"

#include <iostream>

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpRequest::HttpRequest() : _headersReceived(false) {}
HttpRequest::HttpRequest(HttpRequest const &httpRequest) { *this = httpRequest; }

HttpRequest::~HttpRequest() {}

// ############## PRIVATE ##############

std::string HttpRequest::concatenateDataReceived(std::string request) {
	std::string str;
	for (std::vector<std::string>::iterator it = _inReceive.begin(); it != _inReceive.end(); it++)
		str += *it;
	return str.empty() ? str : str + request;
}

// ############## PROTECTED ##############

void HttpRequest::parseFirstLine(std::string firstLine) {
	size_t pos = 0;
	while ((pos = firstLine.find(' ')) != std::string::npos) {
		if (_method.empty())
			_method = firstLine.substr(0, pos);
		else if (_path.empty())
			_path = firstLine.substr(0, pos);
		firstLine.erase(0, pos + 1); //coupe jusqu'a apres l'espace
	}
	_httpVersion = firstLine;
}

bool HttpRequest::parseHeaders(std::string headers) {
	size_t endLinePos = 0;
	while ((endLinePos = headers.find("\r\n")) != std::string::npos) {
		size_t separator = headers.find(':');
		addHeader(headers.substr(0, separator), headers.substr(separator + 2, endLinePos - separator - 2)); // skips ": " and stops before CRLF
		headers.erase(0, endLinePos + 2); // +2 skips CRLF
		//we got to the end of headers.
		if (headers.rfind("\r\n", 0)) { // does the string start with \r\n
			_headersReceived = true;
			if (headers.size() == 2) //theres no body after the headers
				return false;
			_inReceive.push_back(headers.substr(2));
			return true;
		}
	}
}

void HttpRequest::parseBody(std::string messageBody) {
	if (messageBody.empty())
		return ;
	if (!isChunked())
}

void HttpRequest::readChunked(std::string body) {

}

// ############## PUBLIC ##############

std::string HttpRequest::build() {
	std::stringstream ss;
	ss << _method << " ";
	ss << _path << " ";
	ss << _httpVersion << "\r\n";
	ss << Message::build();
	return ss.str();
}

void HttpRequest::parse(std::string request) {
	if (!_headersReceived) {
		if (request.find("\r\n\r\n") == std::string::npos) {
			_inReceive.push_back(request);
		} else {
			std::string str = concatenateDataReceived(request);
			_inReceive.clear();
			parseFirstLine(str.substr(0, str.find("\r\n")));
			parseHeaders(str.substr(str.find("\r\n") + 2));
		}
	}
	if (_headersReceived)
		parseBody(concatenateDataReceived());
}


// ############## GETTERS / SETTERS ##############

void HttpRequest::setMethod(std::string method) {
	_method = method;
}

void HttpRequest::setPath(std::string path) {
	_path = path;
}

std::string HttpRequest::getMethod() const {
	return _method;
}

std::string HttpRequest::getPath() const {
	return _path;
}

		
// ############## OPERATORS ##############

HttpRequest &HttpRequest::operator=(HttpRequest const &rhs) {
	if (this != &rhs) {
		_inReceive = rhs._inReceive;
		_method = rhs._method;
		_path = rhs._path ;
		_httpVersion = rhs._httpVersion;
		_headersReceived = rhs._headersReceived;
		Message::operator=(rhs);
	}
	return *this;
}