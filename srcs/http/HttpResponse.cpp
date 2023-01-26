#include "HttpResponse.hpp"
# include "CGI.hpp"

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
	std::cout << "HERE IN BUILD" << std::endl;
	std::stringstream ss;
	ss << _httpVersion << " ";
	ss << _statusCode << " ";
	ss << _reasonPhrase << "\r\n";
	// ss << _date << "";
	ss << Message::build();
	return ss.str();
}

void HttpResponse::send(Client &client) {
	std::string response = build();
	ws::log(ws::LOG_LVL_ALL, "[HTTP RESPONSE] -", ws::itos(response.length()) + " chars were sent to fd: " + ws::itos(client.getSocket().getFd()));
	ws::log(ws::LOG_LVL_DEBUG, "[HTTP RESPONSE] -", "contents:\n----------\n" + response + "\n----------");
	write(client.getSocket().getFd(), response.c_str(), response.length());
}

void HttpResponse::get_response(HttpRequest &request, Server &server)
{
	if (request.getIsCgi() != "")
	{
		Cgi cgi(request, server);
		int i = 0;
		int j = _response.size() - 2;
		_date = setDate();
		_response = cgi.execute(request.getIsCgi());
	}
	else
	{
		showErrorPage();
	}
}

void	HttpResponse::post_response(HttpRequest &request, Server &server)
{
	if (request.getIsCgi() != "")
	{
		Cgi cgi(request, server);
		int i = 0;
		int j = _response.size() - 2;
		_date = setDate();
		_response = cgi.execute(request.getIsCgi());
	}
	else
	{
		showErrorPage();
	}
}

//cgi ne concerne pas delete method

// void	HttpResponse::delete_response(HttpRequest &request, Server &server)
// {

// }

void	HttpResponse::showErrorPage()
{
	std::string page;

	page = "www/error_page/" + ws::itos(500) + ".html"; // pour le test mais interer ca avec la map d'error.
}

// ############## GETTERS / SETTERS ##############

std::string	HttpResponse::setDate(void)
{
	std::cout << "THIS IS THE HOUR" << std::endl;
	char			buffer[100];
	struct timeval	tv;
	struct tm		*gmt;

	gettimeofday(&tv, NULL);
	gmt = gmtime(&tv.tv_sec);
	strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", gmt);
	return buffer;
}

std::string	HttpResponse::getResponse() const
{
	return _response;
}


// ############## OPERATORS ##############

HttpResponse &HttpResponse::operator=(HttpResponse const &rhs) {
	if (this != &rhs) {
	}
	return *this;
}
