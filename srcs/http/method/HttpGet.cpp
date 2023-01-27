#include "HttpGet.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpGet::HttpGet() {}
HttpGet::HttpGet(HttpGet const &httpGet) { *this = httpGet; }
HttpGet::~HttpGet() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############


HttpResponse HttpGet::execute(ServerInfo const &info, HttpRequest &request) {
	return HttpResponse();
}

HttpMethod *HttpGet::clone() {
	return new HttpGet();
}


std::string HttpGet::getName() {
	return "GET";
}

// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

HttpGet &HttpGet::operator=(HttpGet const &rhs) {
	if (this != &rhs) {
		
	}
	return *this;
}
