#include "HttpResponse.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpResponse::HttpResponse() {}
HttpResponse::HttpResponse(HttpResponse const &httpResponse) { *this = httpResponse; }

HttpResponse::~HttpResponse() {

}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

std::string HttpResponse::build() const {
	
}

// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

HttpResponse &HttpResponse::operator=(HttpResponse const &rhs) {
	if (this != &rhs) {
	}
	return *this;
}
