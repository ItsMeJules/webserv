#include "CGI.hpp"
#include <sys/types.h>
#include <sys/wait.h>

Cgi::Cgi(){}

Cgi::Cgi(Server const &server, HttpRequest &request, std::string binary, std::string target) : _binary(binary), _target(target), _httpRequest(request)
{
	std::vector<std::string> tmp = request.getData();
	std::string content_type = "text/html";

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
	_inputBody = request.getMessageBody()->getBody();


	std::vector<std::string> tmp1 = request.getData();
	for (std::vector<std::string>::iterator it = tmp1.begin(); it != tmp1.end(); it++)
	{
		if (it->find(":") != std::string::npos)
		{
			std::vector<std::string> split = ft_split(*it, ": ");
			ft_to_upper(split[0]);
			split[0] = replace(split[0], "-", "_");
			_env["HTTP_" + split[0]] = split[1];
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

}

Cgi::~Cgi() {}

std::map<std::string, std::string> createCGIMap(HttpRequest &request, Server const &server)
{
	std::string content_type = "text/html";
	std::map<std::string, std::string> env;
	std::string _binary;
	//Pour GET, la seule variable est QUERY STRING

	return env;
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

char **Cgi::envToTab(void)
{
	char **env = new char *[_env.size() + 1];
	std::string tmp;
	int i;
	i = 0;

	for(std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++)
	{
		tmp = it->first + "=" + it->second;
		env[i] = new char[tmp.size() + 1];
		ft_strcpy(tmp.c_str(), env[i]);
	}
	env[i] = NULL;
	return env;
}

std::string	Cgi::execute(void)
{
	std::string _body;
	pid_t	pid;
	int		ret = 1;
	char	tmp[BUFFER_SIZE + 1];
	int		fd[2];

	fd[0] = dup(STDIN_FILENO);
	fd[1] = dup(STDOUT_FILENO);

	FILE *input_file = tmpfile();
	FILE *output_file = tmpfile();

	int	input_fd = fileno(input_file);
	int	output_fd = fileno(output_file);

	write(input_fd, _inputBody.c_str(), _inputBody.length());
	lseek(input_fd, 0, SEEK_SET);


	if (pid == -1)
		return "500"; // ou alors faire un truc avec write response de status code idk
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
		execve(_binary.c_str(), av, envToTab());//il faut crer une fonction qui retourne un tab
		return "500";
	}
	else
	{
		waitpid(-1, NULL, 0);
		lseek(output_fd, 0, SEEK_SET);
		while (ret > 0)
		{
			ft_memset(tmp, 0, BUFFER_SIZE);
			ret = read(output_fd, tmp, BUFFER_SIZE);
			tmp[BUFFER_SIZE] = '\0';
			_body += tmp;
		}
		close(output_fd);
		close(input_fd);
		dup2(fd[0], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
	}
	return _body;

}



