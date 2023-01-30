#include "CGI.hpp"
# include <ostream>

//http://www.wijata.com/cgi/cgispec.html#4.0  GO GO GO

Cgi::Cgi(){}

Cgi::Cgi(HttpRequest &request, Server &server, ServerInfo &serverInfo) : _inputBody(request.getMessageBody()->getBodyStr())
{
	this->initEnv(request, server, serverInfo);
}

Cgi	&Cgi::operator=(Cgi const &src)
{
	if (this != &src)
	{
		_inputBody = src._inputBody;
		_env = src._env;
	}
	return *this;
}

void	Cgi::initEnv(HttpRequest &request, Server &server, ServerInfo &serverInfo)
{
	std::map<std::string, std::string> headers = request.getHeaders();
	std::string content_type = "text/html";

	if (headers.find("Auth-Scheme") != headers.end() && headers["Auth-Scheme"] != "")
		_env["AUTH_TYPE"] = headers["Authorization"];
	_env["STATUS_CODE"] = "200"; // can't be another one
	_env["CGI"] = "CGI/1.1";
	_env["SCRIPT_NAME"] = "./www/cgi/script.php";
	_env["REQUEST_METHOD"] = request.getMethod()->getName();
	_env["CONTENT_LENGTH"] = ws::itos(_inputBody.length());
	_env["CONTENT_TYPE"] = content_type;
	_env["PATH_INFO"] = "./www/cgi/script.php";
	_env["QUERY_STRING"] = request.getQuery();
	_env["REMOTE_ADDR"] = ws::itos(server.getServerSocket().getPort());
	_env["REMOTE_IDENT"] = headers["Authorization"];
	_env["REMOTE_USER"] = headers["Authorization"];
	_env["REQUEST_URI"] = request.getPath() + request.getQuery();

	if (headers.find("Hostname") != headers.end())
		_env["SERVER_NAME"] = headers["Hostname"];
	else
		_env["SERVER_NAME"] = _env["REMOTE_ADDR"];
	_env["SERVER_PORT"] = ws::itos(server.getServerSocket().getPort());
	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_SOFTWARE"] = "Webservgang";

	_env.insert(serverInfo.getCgis().begin(), serverInfo.getCgis().end());
}

Cgi::~Cgi() {}


char **Cgi::envToTab(void)
{
	char **env = new char *[_env.size() + 1];
	std::cout << "size de env = " << _env.size() << std::endl;
	int i = 0;
	std::cout << "_env[0] = " << _env[0] << std::endl;

	for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++)
	{
		std::cout << "entre dans le for du env to tab" << std::endl;
		std::string tmp = it->first + "=" + it->second;
		std::cout << "TMP DANS ENVTOTAB =" << tmp << std::endl;
		env[i] = new char[tmp.size() + 1];
		env[i] = strcpy(env[i], (char*)tmp.c_str());
	}
	env[i] = NULL;
	return env;
}

std::string	Cgi::execute(const std::string	&_binary)
{
	std::string _body;
	pid_t	pid;
	int		fd[2];
	char 	**env;
	int		ret = 1;

	try
	{
		std::cout << "is in the try" << std::endl;
		env = envToTab();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}

	std::cout << "entre dans cgi" << std::endl;
	FILE *input_file = tmpfile();
	FILE *output_file = tmpfile();

	int	input_fd = fileno(input_file);
	int	output_fd = fileno(output_file);

	write(input_fd, _inputBody.c_str(), _inputBody.length());
	lseek(input_fd, 0, SEEK_SET);

	pid = fork();

	if (pid == -1)
	{
		std::cerr << "CRASH" << std::endl;
		return "500";
	}
	else if (!pid)
	{
		char * const * nll = NULL;
		dup2(input_fd, STDIN_FILENO);
		dup2(output_fd, STDOUT_FILENO);
		execve("/usr/bin/php-cgi", nll, env);
		std::cout << "BINARY = " << _binary << std::endl;
		std::cerr << "crash de execve" << std::endl;
		return "500";
	}
	else
	{
		char	tmp[BUFFER_SIZE] = {0};

		waitpid(-1, NULL, 0);
		lseek(output_fd, 0, SEEK_SET);

		ret = 1;
		while (ret > 0)
		{
			memset(tmp, 0, BUFFER_SIZE);
			ret = read(output_fd, tmp, BUFFER_SIZE - 1);
			_body += tmp;
		}
		dup2(fd[0], STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(output_fd);
		close(input_fd);

	}
	for (int i = 0; env[i]; i++)
		delete[] env[i];
	delete[] env;

	if (!pid)
		exit(0);

	return _body;
}


void	Cgi::setInputBody(std::string inputBody)
{
	_inputBody = inputBody;
}

void	Cgi::setEnv(std::map<std::string, std::string> env)
{
	_env = env;
}
std::map<std::string, std::string> Cgi::getEnv() const
{
	return _env;
}

std::string	Cgi::getInputBody() const
{
	return _inputBody;
}

std::ostream &			operator<<( std::ostream & o, Cgi const & i )
{
	o << "INPUT BODY = " << i.getInputBody() << std::endl;
	return o;
}
