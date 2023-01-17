#include "StatusCode.hpp"
#include "assert.h"


StatusCode::~StatusCode(){}
StatusCode::StatusCode(HttpRequest request) : http_request(request) {}

StatusCode::StatusCode()
{
	//info
	_statusCode[100] = "Continue";
	_statusCode[101] = "Switching Protocols";

	//succes
	_statusCode[200] = "OK";
	_statusCode[201] = "Created";
	_statusCode[202] = "Accepted";
	_statusCode[203] = "Non-Authoritative Information";
	_statusCode[204] = "No Content";
	_statusCode[205] = "Reset Content";
	_statusCode[206] = "Partial Content";

	//redirection
	_statusCode[300] = "Multiple Choices";
	_statusCode[301] = "Moved Permanently";
	_statusCode[302] = "Found";
	_statusCode[303] = "See Other";
	_statusCode[304] = "Not Modified";
	_statusCode[305] = "Use proxy";
	_statusCode[307] = "Temporary redirect";

	//client error
	_statusCode[400] = "Bad Request";
	_statusCode[401] = "Unauthorized";
	_statusCode[402] = "Payment Required";
	_statusCode[403] = "Forbidden";
	_statusCode[404] = "Not Found";
	_statusCode[405] = "Method non allowed";
	_statusCode[406] = "Non Acceptable";
	_statusCode[407] = "Proxy Identification Required";
	_statusCode[408] = "Request Timeout";
	_statusCode[409] = "Conflict";
	_statusCode[410] = "Gone";
	_statusCode[411] = "Length Required";
	_statusCode[412] = "Precondition failed";
	_statusCode[413] = "Payload Too Large";
	_statusCode[414] = "URI too long";
	_statusCode[415] = "Unsupported Media Type";
	_statusCode[416] = "Range Not Satisfiable";
	_statusCode[417] = "Expectation Failed";
	_statusCode[426] = "Upgrade Required";

	//server error
	_statusCode[500] = "Internal Server Error";
	_statusCode[501] = "Not Implemented";
	_statusCode[502] = "Bad Gateway";
	_statusCode[503] = "Service Unavailable";
	_statusCode[504] = "Gateway Timeout";
	_statusCode[505] = "HTTP Version Non supported";
};

HttpResponse	StatusCode::createResponse(int errorCode, RegularBody * body)
{
    std::string message = _statusCode[errorCode];
    HttpResponse response("HTTP/1.1", errorCode, message);
	body->append(message);
    return response;
}

void	StatusCode::writeResponse()
{
	HttpResponse reponse;
	HttpRequest request;
	RegularBody *body = new RegularBody();
	StatusCode statusCode;
	std::string _path;

	if (request.getMethod() != "GET")
		reponse = statusCode.createResponse(405, body);
	else if (request.getMethod() != "POST")
		reponse = statusCode.createResponse(405, body);
	else if (request.getMethod() != "DELETE")
		reponse = statusCode.createResponse(405, body);
	else if (request.getPath() == "")
		reponse = statusCode.createResponse(404, body);
	else if (request.getPath().empty())
		reponse = statusCode.createResponse(404, body);
	//rajouter une condition si GCI n'est pas ok. Ce sera une erreur 500 car erreur de server
}

std::string &StatusCode::operator[](int status_code) {
  return _statusCode[status_code];
}

StatusCode	&StatusCode::operator=(StatusCode const &rhs)
{
	if (this != &rhs)
	{
		this->_statusCode = rhs._statusCode;
		this->http_request = rhs.http_request;
	}
	return *this;
}

