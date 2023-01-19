#include "HttpResponse.hpp"

std::map<int, ws::http_status_t> HttpResponse::codes = HttpResponse::createCodes();

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpResponse::HttpResponse(std::string httpVersion, int statusCode)
	: Message(httpVersion), _statusCode(statusCode), _statusPhrase(codes[statusCode]) {}

HttpResponse::HttpResponse(int statusCode)
	: Message("HTTP/1.1"), _statusCode(statusCode), _statusPhrase(codes[statusCode]) {}


HttpResponse::HttpResponse() : _statusCode(200), _statusPhrase(codes[200]) {}
HttpResponse::HttpResponse(HttpResponse const &httpResponse) { *this = httpResponse; }
HttpResponse::~HttpResponse() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

std::string HttpResponse::build() {
	std::stringstream ss;
	ss << _httpVersion << " ";
	ss << _statusCode << " ";
	ss << _statusPhrase.reason << "\r\n";
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
        _statusPhrase = rhs._statusPhrase;
	}
	return *this;
}

HttpResponse HttpResponse::fromRequest(ServerInfo const &serverInfo, HttpRequest const &request) {
    ResponseBuilder builder(serverInfo, request);

    return HttpResponse(builder.getStatusCode());
}

std::map<int, ws::http_status_t> HttpResponse::createCodes() {
    std::map<int, ws::http_status_t> codes;

    // succes
    codes[200] = {"OK", ""};
    codes[201] = {"Created", ""};

    // redirection
    codes[300] = {"Multiple Choices", "The requested resource has multiple options available. Please choose one of the options below:"};
    codes[301] = {"Moved Permanently", "The requested document you're looking for was moved permanently."};
    codes[302] = {"Found", "The old requested document was moved, the new location was found."};
    codes[303] = {"See Other", "The resource has been moved."};

    // client error
    codes[400] = {"Bad Request", "The request received is invalid! Please review the request sent by your client."};
    // codes[401] = {"Unauthorized", "Unauthorized request. Auth credentials not valid."};
    codes[403] = {"Forbidden", "You are not authorized to access the requested resource."};
    codes[404] = {"Not Found", "The page you are looking for was not found."};
    codes[405] = {"Method not allowed", "You can't use this HTTP method on this resource."};
    codes[413] = {"Payload Too Large", "The payload received exceeds the server's limit."};

    // server error
    codes[500] = {"Internal Server Error", "The server had some unexpected error."};
    codes[501] = {"Not Implemented", "The HTTP method is not yet supported."};
    codes[505] = {"HTTP Version Non supported", "The HTTP version is not yet supported."};
    return codes;
}