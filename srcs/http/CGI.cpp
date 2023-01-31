#include "CGI.hpp"
# include <ostream>

//http://www.wijata.com/cgi/cgispec.html#4.0  GO GO GO

Cgi::Cgi(){}

Cgi::Cgi(std::map<std::string, std::string> const &cgis) : _cgis(cgis)
{
}

Cgi::~Cgi(){}

Cgi	&Cgi::operator=(Cgi const &src)
{
	if (this != &src)
	{
		_env = src._env;
		_cgis = src._cgis;
	}
	return *this;
}

std::string ftostr(std::string loc)
{
	std::string buffer;

	std::ifstream fin(loc.c_str());
	getline(fin, buffer, char(-1));
	fin.close();
	return buffer;
}

char **Cgi::generateEnv()
{
	char **env = new char*[_env.size() + 1];
	int i = 0;

	for (std::map<std::string, std::string>::iterator it = _env.begin(); it != _env.end(); it++) {
		std::string str = it->first + "=" + it->second;

		env[i] = strdup(str.c_str());
		env[i][str.size()] = 0;
		i++;
	}
	env[i] = NULL;
	return env;
}

std::string	Cgi::executeGet(HttpRequest &request, ws::request_data_t &data)
{
	std::cout << "arrive dans executeget" << std::endl;
	int redirFd;
	ws::tmp_file_t tmpFile;
	std::ofstream tmpStream;

	_env["REQUEST_METHOD"] = "GET";
	_env["STATUS_CODE"] = "200";
	_env["PATH_INFO"] = "./www/cgi/script.py";
	_env["PATH_TRANSLATED"] = "./www/cgi/script.php";
	_env["PATH_NAME"] = "./www/cgi/script.php";
	_env["SCRIPT_NAME"] = "./www/cgi/script.php";
	_env["SCRIPT_FILENNAME"] = "./www/cgi/script.php";
	_env["QUERY_STRING"] = data.query;
	_env["CONTENT_LENGTH"] = ws::itos(request.getPath().length());

	char **env = new char*[3];
	char **cgiEnv = generateEnv();

	env[0] = &_cgis[data.fileExtension][0];
	env[1] = &data.requestedPath[0];
	env[2] = NULL;

	ws::make_tmp_file(tmpFile);

	tmpStream.open(tmpFile.name.c_str(), std::ofstream::out | std::ofstream::app);
	redirFd = open("emmacCGI", O_CREAT | O_WRONLY | O_TRUNC, 0666);

	int pid = fork();
	if (pid == 0)
	{
		dup2(tmpFile.fd, 0);
		ws::close_tmp_file(tmpFile);
		tmpStream.close();

		dup2(redirFd, 1);
		close(redirFd);

		execve(_cgis[data.fileExtension].c_str(), env, cgiEnv);
	}
	else
		wait(NULL);

	delete[] env;
	delete[] cgiEnv;
	return (ftostr("emmacCGI"));
}
