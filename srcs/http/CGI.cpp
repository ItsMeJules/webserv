#include "CGI.hpp"
#include "utils.hpp"

Cgi::Cgi(){}

Cgi::Cgi(Server const &server, HttpRequest const &request, std::string binary, std::string target)
{
	// std::vector<std::string> tmp = request.getData();

	// for (std::vector<std::string>::iterator it = tmp.begin(); it != tmp.end() ; it++)
	// {
	// 	if (it->find("Content-type: ") != std::string::npos)
	// 	{
	// 		content_type = it->substr(14, it->length() - 14);
	// 		break;
	// 	}
	// }


	// std::vector<std::string> tmp1 = request.getData();
	// for (std::vector<std::string>::iterator it = tmp1.begin(); it != tmp1.end(); it++)
	// {
	// 	if (it->find(":") != std::string::npos)
	// 	{
	// 		// std::vector<std::string> split = ft_split(*it, ": ");
	// 		// ft_to_upper(split[0]);
	// 		// split[0] = rep
	// 	}
	// }


}

std::string getCGIExecutable(std::string uri) {
	std::string cgiPath = "/usr/local/bin/";
	std::string cgiExecutable = cgiPath + uri + ".cgi";
	return cgiExecutable;
}

std::map<std::string, std::string> createCGIMap(HttpRequest &request, Server const &server)
{
	std::string content_type = "text/html";
	std::map<std::string, std::string> env;
	std::string _binary;

	env["STATUS_CODE"] = "200";
	env["CGI"] = "CGI/1.1";
	env["SCRIPT_NAME"] = _binary;
	env["REQUEST_METHOD"] = request.getMethod();
	env["CONTENT_TYPE"] = content_type;
	env["SERVER_PROTOCOL"] = "HTTP/1.1";
	env["SERVER_NAME"] = server.getName();
	env["REMOTE_USER"] = "user";

	return env;
}

void handleCGIRequest(Server &server, HttpRequest &request, int clientSocket)
{
	std::string binary = getCGIExecutable(request.getHttpVersion());
	std::string target = request.getHttpVersion();
	std::string inputBody = request.getMessageBody()->getBody();
	std::map<std::string, std::string> env = createCGIMap(request, server);
	Cgi cgi(server, request, binary, target);
	cgi.setInputBody(inputBody);
	cgi.setEnv(env);

	int response = cgi.execute(clientSocket);


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

void	Cgi::setInputBody(std::string inputBody)
{
	_inputBody = inputBody;
}

void	Cgi::setEnv(std::map<std::string, std::string> env)
{
	_env = env;
}
