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
	ws::log(ws::LOG_LVL_ALL, "[HTTP RESPONSE] -", ws::itos(response.length()) + " chars were sent to fd: " + ws::itos(client.getSocket().getFd()));
	ws::log(ws::LOG_LVL_DEBUG, "[HTTP RESPONSE] -", "contents:\n----------\n" + response + "\n----------");
	write(client.getSocket().getFd(), response.c_str(), response.length());
}

void	HttpResponse::cgi(void)
{

}

// ############## GETTERS / SETTERS ##############

std::string	HttpResponse::setDate(void)
{
	struct timeval	time;
	struct tm		*gmt;
	char *buffer;

	
}

// ############## OPERATORS ##############

HttpResponse &HttpResponse::operator=(HttpResponse const &rhs) {
	if (this != &rhs) {
	}
	return *this;
}
