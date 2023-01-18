#include "HttpResponse.hpp"

std::map<int, HttpResponse::http_error_t> HttpResponse::codes = HttpResponse::createCodes();

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

// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

HttpResponse &HttpResponse::operator=(HttpResponse const &rhs) {
	if (this != &rhs) {
        _statusCode = rhs._statusCode;
        _reasonPhrase = rhs._reasonPhrase;
	}
	return *this;
}

HttpResponse HttpResponse::fromRequest(ServerInfo const &serverInfo, HttpRequest const &request) {
    return HttpResponse("HTTP/1.1", 200, codes[200].reasonPhrase);
}

std::map<int, HttpResponse::http_error_t> HttpResponse::createCodes() {
    std::map<int, std::string> codes;

    //succes
    codes[200] = {"OK", ""};
    codes[201] = {"Created", ""};

    //redirection
    codes[300] = {"Multiple Choices", "The requested resource has multiple options available. Please choose one of the options below:"};
    codes[301] = {"Moved Permanently", "The requested document you're looking for was moved permanently."};
    codes[302] = {"Found", "The old requested document was moved, the new location was found."};
    codes[303] = "See Other";
    codes[304] = "Not Modified";
    codes[305] = "Use proxy";
    codes[307] = "Temporary redirect";

    //client error
    codes[400] = "Bad Request";
    codes[401] = "Unauthorized";
    codes[402] = "Payment Required";
    codes[403] = "Forbidden";
    codes[404] = "Not Found";
    codes[405] = "Method non allowed";
    codes[406] = "Non Acceptable";
    codes[407] = "Proxy Identification Required";
    codes[408] = "Request Timeout";
    codes[409] = "Conflict";
    codes[410] = "Gone";
    codes[411] = "Length Required";
    codes[412] = "Precondition failed";
    codes[413] = "Payload Too Large";
    codes[414] = "URI too long";
    codes[415] = "Unsupported Media Type";
    codes[416] = "Range Not Satisfiable";
    codes[417] = "Expectation Failed";
    codes[426] = "Upgrade Required";

//server error
    codes[500] = "Internal Server Error";
    codes[501] = "Not Implemented";
    codes[502] = "Bad Gateway";
    codes[503] = "Service Unavailable";
    codes[504] = "Gateway Timeout";
    codes[505] = "HTTP Version Non supported";
    return codes;
}