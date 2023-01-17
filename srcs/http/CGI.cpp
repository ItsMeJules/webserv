#include "CGI.hpp"
#include "utils.hpp"
#include <sys/types.h>
#include <sys/wait.h>

Cgi::Cgi(){}

Cgi::Cgi(Server const &server, HttpRequest const &request, std::string binary, std::string target) : _binary(binary), _target(target), _httpRequest(request)
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

	char **arg;


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
	//Pour GET, la seule variable est QUERY STRING

	return env;
}


void handleCGIRequest(Server &server, HttpRequest &request, int clientSocket)
{
	std::string binary = getCGIExecutable(request.getPath());
	std::string target = request.getPath();
	std::string inputBody = request.getMessageBody()->getBody();
	std::map<std::string, std::string> env = createCGIMap(request, server);
	Cgi cgi(server, request, binary, target);
	cgi.setInputBody(inputBody);
	cgi.setEnv(env);

	int response = cgi.execute(clientSocket);
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

int	Cgi::execute(int clientSocket)
{
	std::string _body;
	pid_t	pid;
	int		ret = 1;
	int		fd[2];

	fd[0] = dup(STDIN_FILENO);
	fd[1] = dup(STDOUT_FILENO);

	FILE *input_file = tmpfile();
	FILE *output_file = tmpfile();

	int	input_fd = fileno(input_file);
	int	output_fd = fileno(output_file);

	if (pid == -1)
		return 500; // ou alors faire un truc avec write response de status code idk
	else if (pid == 0)
	{
		char **av = new char * [3];
		av[0] = new char [_binary.length() + 1];
		av[1] = new char [_target.length() + 1];

		dup2(output_fd, STDOUT_FILENO);
		dup2(input_fd, STDIN_FILENO);

		ft_strcpy(_binary.c_str(), av[0]);
		ft_strcpy(_target.c_str(), av[1]);
		av[2] = NULL;
		execve(_binary.c_str(), av, );//il faut crer une fonction qui retourne un tab
	}
	else
	{
	}

}
