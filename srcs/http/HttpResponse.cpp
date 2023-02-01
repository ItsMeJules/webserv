#include "HttpResponse.hpp"

std::map<int, ws::http_status_t> HttpResponse::codes = HttpResponse::createCodes();

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpResponse::HttpResponse(int statusCode)
	: HttpMessage("HTTP/1.1"), _statusCode(statusCode), _statusPhrase(codes[statusCode]) {}

HttpResponse::HttpResponse() : HttpMessage("HTTP/1.1"), _statusCode(200), _statusPhrase(codes[200]) {}
HttpResponse::HttpResponse(HttpResponse const &httpResponse) { *this = httpResponse; }
HttpResponse::~HttpResponse() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

const std::string HttpResponse::build() const {
	std::stringstream ss;
	ss << _httpVersion << " ";
	ss << _statusCode << " ";
	ss << _statusPhrase.reason << "\r\n";
	ss << HttpMessage::build();
	return ss.str();
}

std::string	HttpResponse::generateDate(void)
{
	char			buffer[100];
	struct timeval	tv;
	struct tm		*gmt;

	gettimeofday(&tv, NULL);
	gmt = gmtime(&tv.tv_sec);
	strftime(buffer, 100, "%a, %d %b %Y %H:%M:%S GMT", gmt);
	return buffer;
}

void HttpResponse::generateError(int code, std::map<int, std::string> const &errorPage, DefaultBody &body) {
	std::ifstream fileStream(errorPage.at(code).c_str());

    setStatusCode(code);
    if (fileStream.is_open()) {
        size_t fileSize = ws::get_file_size(fileStream);

        body.append(ws::get_file_contents(fileStream, fileSize), fileSize);
    } else
        body.append(_statusPhrase.explanation, _statusPhrase.explanation.size());

    addHeader("Content-Type", "text/html");
    if (code != 404)
        addHeader("Connection", "close"); // maybe the connection doesn't always get closed (example 404 errors)
}

// ############## GETTERS / SETTERS ##############

void HttpResponse::setStatusCode(int statusCode) {
    _statusCode = statusCode;
    _statusPhrase = codes[_statusCode];
}

const int &HttpResponse::getStatusCode() const {
    return _statusCode;
}

// ############## OPERATORS ##############

HttpResponse &HttpResponse::operator=(HttpResponse const &rhs) {
	if (this != &rhs) {
        _statusCode = rhs._statusCode;
        _statusPhrase = rhs._statusPhrase;
        HttpMessage::operator=(rhs);
	}
	return *this;
}

ws::http_status_t HttpResponse::createStatus(std::string reason, std::string explanation) {
    ws::http_status_t status;

    status.reason = reason;
    status.explanation = explanation;
    return status;
}

std::map<int, ws::http_status_t> HttpResponse::createCodes() {
    std::map<int, ws::http_status_t> codes;

    // succes
    codes[200] = createStatus("OK", "Everything went well.");
    codes[201] = createStatus("Created", "Content was created.");
    codes[204] = createStatus("No content", "The ressource has been deleted");

    // redirection
    codes[300] = createStatus("Multiple Choices", "The requested resource has multiple options available. Please choose one of the options below:");
    codes[301] = createStatus("Moved Permanently", "The requested document you're looking for was moved permanently.");
    codes[302] = createStatus("Found", "The old requested document was moved, the new location was found.");
    codes[303] = createStatus("See Other", "The resource has been moved.");

    // client error
    codes[400] = createStatus("Bad Request", "The request received is invalid! Please review the request sent by your client.");
    // codes[401] = createStatus("Unauthorized", "Unauthorized request. Auth credentials not valid.");
    codes[403] = createStatus("Forbidden", "You are not authorized to access the requested resource.");
    codes[404] = createStatus("Not Found", "The page you are looking for was not found.");
    codes[405] = createStatus("Method not allowed", "You can't use this HTTP method on this resource.");
    codes[413] = createStatus("Payload Too Large", "The payload received exceeds the server's limit.");

    // server error
    codes[500] = createStatus("Internal Server Error", "The server had some unexpected error.");
    codes[501] = createStatus("Not Implemented", "The HTTP method is not yet supported.");
    codes[505] = createStatus("HTTP Version Non supported", "The HTTP version is not yet supported.");
    return codes;
}