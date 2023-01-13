#include "CGI.hpp"

Cgi::Cgi(){}

Cgi::Cgi(Server const &server, HttpRequest const &request, std::string binary, std::string target)
{
	std::string content_type = "text/html";
	// int content_length = request.

	_env["STATUS_CODE"] = "200";
	_env["CGI"] = "CGI/1.1";
	_env["SCRIPT_NAME"] = _binary;
	_env["REQUEST_METHOD"] = request.getMethod();
	_env["CONTENT_TYPE"] = content_type;
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_NAME"] = server.getName();
	_env["REMOTE_USER"] = "user";

}


std::string	Cgi::int_to_string(int i)
{
	std::stringstream ss;
	ss << i;
	std::string s = ss.str();
	return s;
}

