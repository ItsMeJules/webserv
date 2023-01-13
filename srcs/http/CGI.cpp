#include "CGI.hpp"

Cgi::Cgi(Server const &server, HttpRequest const &request, std::string binary, std::string target)
{
	_env["STATUS_CODE"] = "200";
	_env["CGI"] = "CGI/1.1";
	_env["SCRIPT_NAME"] = _binary;
	_env["REQUEST_METHOD"] = request.getMethod();
	_env["CONTENT_LENGTH"] = int_to_string(HttpRequest.getBody().length());
	_env["REQUEST_URL"]
}

Cgi::~Cgi() {}


