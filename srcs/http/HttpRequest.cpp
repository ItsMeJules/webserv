#include "HttpRequest.hpp"

#include <iostream>

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpRequest::HttpRequest() : _parser(*this) {}
HttpRequest::HttpRequest(HttpRequest const &httpRequest) : _parser(*this) { *this = httpRequest; }

HttpRequest::~HttpRequest() {}

// ############## PRIVATE ##############

// ############## PROTECTED ##############

// ############## PUBLIC ##############

std::string HttpRequest::build() {
	std::stringstream ss;
	ss << _method << " ";
	ss << _path << " ";
	ss << _httpVersion << "\r\n";
	ss << Message::build();
	return ss.str();
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

RequestParser &HttpRequest::getParser() {
	return _parser;
}
		
// ############## OPERATORS ##############

HttpRequest &HttpRequest::operator=(HttpRequest const &rhs) {
	if (this != &rhs) {
		_method = rhs._method;
		_path = rhs._path ;
		Message::operator=(rhs);
	}
	return *this;
}