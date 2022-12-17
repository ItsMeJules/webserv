#include "HttpResponse.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpResponse::HttpResponse() {}
HttpResponse::HttpResponse(std::string httpVersion, int statusCode, std::string reasonPhrase)
	: Message(httpVersion), _statusCode(statusCode), _reasonPhrase(reasonPhrase) {}
HttpResponse::HttpResponse(HttpResponse const &httpResponse) { *this = httpResponse; }

HttpResponse::~HttpResponse() {

}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

std::string HttpResponse::build() {
	std::stringstream ss;
	ss << _httpVersion << " ";
	ss << _statusCode << " ";
	ss << _reasonPhrase << "\r\n";
	ss << Message::build();
	return ss.str();
}

void HttpResponse::send(Client &client) {
	std::string response = build();

	std::cout << response << std::endl;
	write(client.getSocket().getFd(), response.c_str(), response.length());
}

// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

HttpResponse &HttpResponse::operator=(HttpResponse const &rhs) {
	if (this != &rhs) {
	}
	return *this;
}
