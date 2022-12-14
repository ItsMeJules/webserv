#include "HttpRequest.hpp"
#include "HttpRequest.hpp"

#include <iostream>

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpRequest::HttpRequest() {}
HttpRequest::HttpRequest(std::string method, std::string path, std::string httpVersion) 
	: _method(method), _path(path), _httpVersion(httpVersion) {}
HttpRequest::HttpRequest(HttpRequest const &httpRequest) { *this = httpRequest; }

HttpRequest::~HttpRequest() {

}

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

void HttpRequest::parseHeaders(std::string headers) {
	size_t endLinePos = 0;
	while ((endLinePos = headers.find("\r\n")) != std::string::npos) {
		size_t separator = headers.find(':');
		// skips ": " and stops before CRLF
		addHeader(headers.substr(0, separator), headers.substr(separator + 2, endLinePos - separator - 2));
		headers.erase(0, endLinePos + 2); // +2 skips CRLF
		if (headers == "\r\n")
			return ;			
	}
}

void HttpRequest::parseBody(std::string messageBody) {
	std::cout << "body: " << messageBody << std::endl;
}

// ############## PUBLIC ##############

std::string HttpRequest::build() const {
	std::stringstream ss;
	
	return ss.str();
}

void HttpRequest::parse(std::string &request) {
	parseFirstLine(request.substr(0, request.find("\r\n")));
	parseHeaders(request.substr(request.find("\r\n") + 2)); // skips CRLF
	parseBody(request.substr(request.rfind("\r\n") + 2)); // skips CRLF
}


// ############## GETTERS / SETTERS ##############

void HttpRequest::setMethod(std::string method) {
	_method = method;
}

void HttpRequest::setPath(std::string path) {
	_path = path;
}

void HttpRequest::setHttpVersion(std::string httpVersion) {
	_httpVersion = httpVersion;
}


std::string HttpRequest::getMethod() const {
	return _method;
}

std::string HttpRequest::getPath() const {
	return _path;
}

std::string HttpRequest::getHttpVersion() const {
	return _httpVersion;
}

		
// ############## OPERATORS ##############

HttpRequest &HttpRequest::operator=(HttpRequest const &rhs) {
	if (this != &rhs) {
	}
	return *this;
}
