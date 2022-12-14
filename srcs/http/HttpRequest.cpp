#include "HttpRequest.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpRequest::HttpRequest() {}
HttpRequest::HttpRequest(HttpRequest const &httpRequest) { *this = httpRequest; }

HttpRequest::~HttpRequest() {

}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

std::string HttpRequest::build() const {
	
}

// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

HttpRequest &HttpRequest::operator=(HttpRequest const &rhs) {
	if (this != &rhs) {
	}
	return *this;
}
