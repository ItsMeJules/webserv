#include "StatusCode.hpp"
#include "assert.h"


StatusCode::~StatusCode(){}
StatusCode::StatusCode(HttpRequest request) : http_request(request) {}

StatusCode::StatusCode()
{
	_statusCode[200] = "OK";
	_statusCode[400] = "Bad Request";
	_statusCode[404] = "Not Found";
};

HttpResponse	StatusCode::createResponse(StatusCode &status, int errorCode)
{
    std::string message = status[errorCode];
    HttpResponse response("HTTP/1.1", errorCode, message);
    response.setMessageBody(new RegularBody(message));
    return response;
}

std::string &StatusCode::operator[](int status_code) {
  return _statusCode[status_code];
}
