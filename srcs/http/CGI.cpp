#include "CGI.hpp"
#include "utils.hpp"

Cgi::Cgi(){}

Cgi::Cgi(Server const &server, HttpRequest const &request, std::string binary, std::string target)
{
	std::string content_type = "text/html";
	std::vector<std::string> tmp = request.getData();

	for (std::vector<std::string>::iterator it = tmp.begin(); it != tmp.end() ; it++)
	{
		if (it->find("Content-type: ") != std::string::npos)
		{
			content_type = it->substr(14, it->length() - 14);
			break;
		}
	}

	_binary = binary;
	_target = target;

	std::vector<std::string> tmp1 = request.getData();
	for (std::vector<std::string>::iterator it = tmp1.begin(); it != tmp1.end(); it++)
	{
		if (it->find(":") != std::string::npos)
		{
			// std::vector<std::string> split = ft_split(*it, ": ");
			// ft_to_upper(split[0]);
			// split[0] = rep
		}
	}

	_env["STATUS_CODE"] = "200";
	_env["CGI"] = "CGI/1.1";
	_env["SCRIPT_NAME"] = _binary;
	_env["REQUEST_METHOD"] = request.getMethod();
	_env["CONTENT_TYPE"] = content_type;
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_NAME"] = server.getName();
	_env["REMOTE_USER"] = "user";

}

std::string	executeCGI(void)
{
	std::string _body;

}

std::string	Cgi::getBinary() const
{
	return _binary;
}

std::string Cgi::getTarget() const
{
	return _target;
}

