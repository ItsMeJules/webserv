#include "HttpDelete.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpDelete::HttpDelete() {}
HttpDelete::HttpDelete(HttpDelete const &httpDelete) { *this = httpDelete; }
HttpDelete::~HttpDelete() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

HttpResponse HttpDelete::execute(ServerInfo const &info, HttpRequest &request) {
	return HttpResponse();
}

HttpMethod *HttpDelete::clone() {
	return new HttpDelete();
}

std::string HttpDelete::getName() {
	return "DELETE";
}

std::string HttpDelete::getPath() const
{
	return _path;
}

// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

HttpDelete &HttpDelete::operator=(HttpDelete const &rhs) {
	if (this != &rhs) {

	}
	return *this;
}
