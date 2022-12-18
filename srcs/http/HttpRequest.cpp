#include "HttpRequest.hpp"

#include <iostream>

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpRequest::HttpRequest() {}
HttpRequest::HttpRequest(HttpRequest const &httpRequest) { *this = httpRequest; }

HttpRequest::~HttpRequest() {}

// ############## PRIVATE ##############

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
		if (headers == "\r\n") //we got to the end of headers with no body.
			return false;
	}
	return true;
}

void HttpRequest::parseBody(std::string messageBody) {
	_messageBody.append(messageBody);
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
	if (request.find("\r\n\r\n") == std::string::npos) {
		_inReceive.push_back(request);
	} else {
		std::string str;
		if (!_inReceive.empty()) {
			for (std::vector<std::string>::iterator it = _inReceive.begin(); it != _inReceive.end(); it++)
				str += *it;
		}
		str += request;
		parseFirstLine(str.substr(0, str.find("\r\n")));
		if (parseHeaders(str.substr(str.find("\r\n") + 2))) { // skips CRLF
			if (!isChunked())
				std::cout << "chunked!" << std::endl;
			else
				std::cout << "not chunked!" << std::endl;
		}
		_inReceive.clear();
	}
	parseBody(request.substr(request.rfind("\r\n") + 2)); // skips CRLF
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
		Message::operator=(rhs);
	}
	return *this;
}