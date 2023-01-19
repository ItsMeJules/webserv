#include "HttpRequest.hpp"

#include <iostream>

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpRequest::HttpRequest() {}
HttpRequest::HttpRequest(HttpRequest const &httpRequest) { *this = httpRequest; }
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

void HttpRequest::execute(ServerInfo const &serverInfo) {
	FileBody *fileBody = dynamic_cast<FileBody*>(_messageBody);
	if (fileBody != NULL) {
		fileBody->createFile(serverInfo.getRootPath() + serverInfo.getUploadPath());
	}
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
		_method = rhs._method;
		_path = rhs._path;
		Message::operator=(rhs);
	}
	return *this;
}