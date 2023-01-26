#include "HttpRequest.hpp"

std::map<std::string, HttpMethod*> HttpRequest::methods = HttpRequest::initMethods();

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpRequest::HttpRequest() : _method(NULL) {}
HttpRequest::HttpRequest(HttpRequest const &httpRequest) { *this = httpRequest; }
HttpRequest::~HttpRequest() {
	delete _method;
}

// ############## PRIVATE ##############

// ############## PROTECTED ##############

// ############## PUBLIC ##############

const std::string HttpRequest::build() const {
	std::stringstream ss;
	ss << _method->getName() << " ";
	ss << _path << " ";
	ss << _httpVersion << "\r\n";
	ss << HttpMessage::build();
	return ss.str();
}

HttpResponse HttpRequest::execute(ServerInfo const &serverInfo) {
	return _method->execute(serverInfo, *this);
}

// ############## GETTERS / SETTERS ##############

void HttpRequest::setMethod(std::string method) {
	if (HttpRequest::methods.count(method) == 0)
		return ;
	_method = HttpRequest::methods.at(method)->clone();
}

void HttpRequest::setPath(std::string path) {
	_path = path;
}

HttpMethod *HttpRequest::getMethod() const {
	return _method;
}

std::string HttpRequest::getPath() const {
	return _path;
}

std::vector<std::string> HttpRequest::getData() const {
	return _data;
}

std::string	HttpRequest::getQuery() const {
	return _query;
}

std::string HttpRequest::getIsCgi() const {
	return _isCgi;
}

// ############## OPERATORS ##############

HttpRequest &HttpRequest::operator=(HttpRequest const &rhs) {
	if (this != &rhs) {
		_method = rhs._method;
		_path = rhs._path;
		HttpMessage::operator=(rhs);
	}
	return *this;
}

std::map<std::string, HttpMethod*> HttpRequest::initMethods() {
	std::map<std::string, HttpMethod*> map;
	
	map.insert(std::make_pair("GET", new HttpGet()));
	map.insert(std::make_pair("POST", new HttpPost()));
	map.insert(std::make_pair("DELETE", new HttpDelete()));
	return map;
}

void HttpRequest::clearMethods() {
    for (std::map<std::string, HttpMethod*>::const_iterator it = HttpRequest::methods.begin(); it != HttpRequest::methods.end(); it++)
		delete it->second;
}