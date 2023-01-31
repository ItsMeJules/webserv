#include "HttpGet.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpGet::HttpGet() {}
HttpGet::HttpGet(HttpGet const &httpGet) { *this = httpGet; }
HttpGet::~HttpGet() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

std::string	HttpGet::firstPage(std::string filePath)
{
	(void)filePath;
	std::string index;
	std::string path;

	if (filePath.find(".php") != std::string::npos)
	{
		index = execFile(filePath);
		index += "</body>";
		index += "</html";
		return index;
	}
	else if (filePath.find(".py") != std::string::npos)
	{
		index = executeGet();
		index+= "</body>";
		index+= "</html";
		return index;
	}

	std::string recup;
	std::ifstream fileindex(filePath.c_str());
	while (getline(fileindex, recup))
		index+= recup;
	index+= "</body>";
	index+= "</html";
	fileindex.close();
	return index;
}


static int	check_charset(char c, char current)
{
	if (current == c)
	{
		return (1);
	}
	return (0);
}

static int	words_count(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (check_charset(c, s[i]))
		i++;
	while (s[i])
	{
		if (check_charset(c, s[i]))
			count++;
		while (check_charset(c, s[i]))
			i++;
		i++;
	}
	if (!check_charset(c, s[i]))
		count++;
	return (count);
}

static int	wordlen(char const *s, char c)
{
	int	i;
	int	s_size;

	s_size = 0;
	i = 0;
	while (check_charset(c, s[i]) && s[i])
		i++;
	while (!check_charset(c, s[i]) && s[i])
	{
		s_size++;
		i++;
	}
	return (s_size);
}

static char	*dupword(char const *s, char c, int n)
{
	char	*dup;
	int		i;
	int		j;

	i = 0;
	j = 0;
	dup = (char *)malloc(sizeof(char) * (n + 1));
	if (!dup)
		return (NULL);
	while (check_charset(c, s[i]))
		i++;
	while (!check_charset(c, s[i]) && s[i])
	{
		dup[j] = s[i];
		j++;
		i++;
	}
	dup[j] = 0;
	return (dup);
}

char	**ft_split(char const *s, char c)
{
	char	**str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s)
		return (0);
	str = (char **)malloc(sizeof(char *) * (words_count(s, c) + 1));
	if (!str)
		return (NULL);
	while (s[i])
	{
		if (check_charset(c, s[i]))
			i++;
		else
		{
			str[j] = dupword(s + i, c, wordlen(s + i, c));
			j++;
			i += wordlen(s + i, c);
		}
	}
	str[j] = NULL;
	return (str);
}


char	**HttpGet::ft_regroup_envVector(std::vector<char *> vec)
{
	char **res;
	int i = 0;

	res = (char **)malloc(sizeof(char*) * (vec.size() + 1));
	std::vector<char *>::iterator it = vec.begin();
	for (; it != vec.end(); it++)
	{
		res[i] = (char *)*it;
		i++;
	}
	res[i] = NULL;
	return res;
}

void	freeTab2(char **tab)
{
	int i = 0;

	while (tab[i])
	{
		if (tab[i])
		{
			free(tab[i]);
		}
		i++;
	}
	free(tab);
	tab = NULL;
}

std::string ftostr(std::string loc)
{
	std::string buffer;

	std::ifstream fin(loc.c_str());
	getline(fin, buffer, char(-1));
	fin.close();
	return buffer;
}

std::string HttpGet::execFile(std::string file)
{
	int tmp;
	std::string	recup = file;
	tmp = open(recup.c_str(), O_RDONLY, 0644);
	int emma = open("emmaCGI", O_CREAT | O_WRONLY | O_RDONLY | O_TRUNC, 0644);
	char **cmd = new char*[3];
	cmd[0] = strdup("/usr/bin/php-cgi");
	cmd[1] = strdup(file.c_str());
	cmd[2] = 0;

	_vectorEnv.push_back((char *)("REQUEST_METHOD=GET"));
	std::string result = "PATH_INFO=" + file;
	_vectorEnv.push_back((char *)(result.c_str()));
	std::string result1 = "PATH_TRANSLATED=" + file;
	_vectorEnv.push_back((char *)(result1.c_str()));
	std::string result2 = "PATH_NAME=" + file;
	_vectorEnv.push_back((char *)(result2.c_str()));
	std::string result3 = "SCRIPT_NAME=" + file;
	_vectorEnv.push_back((char *)(result3.c_str()));
	std::string result4 = "SCRIPT_FILENAME=" + file;
	_vectorEnv.push_back((char *)(result4.c_str()));
	_env = ft_regroup_envVector(_vectorEnv);

	std::string index;

	int pid = fork();
	if (pid == 0)
	{
		dup2(tmp, 0);
		close(tmp);
		dup2(emma, 1);
		close(emma);
		execve(cmd[0], cmd, _env);
	}
	else
		wait(NULL);
	_vectorEnv.clear();
	_vectorEnv = _vectorEnvCpy;
	if (_env[0])
	{
		freeTab2(_env);
		_env = NULL;
	}
	if (_env)
		freeTab2(_env);
	std::string str1 = ftostr("emmaCGI");
	std::string slip = "Content-type: text/html; charset=UTF-8 ";
	str1 = str1.substr(slip.length(), str1.length());
	free(cmd[0]);
	free(cmd[1]);
	close (tmp);
	unlink(".tmp");
	delete [] cmd;
	return str1;
}
std::string	HttpGet::executeGet(void)
{
	int fd1;

	_vectorEnv.push_back((char*)("REQUEST_METHOD=GET"));
	_vectorEnv.push_back((char*)("PATH_INFO=./www/cgi/script.py"));
	_vectorEnv.push_back((char*)("PATH_TRANSLATED=./www/cgi/script.py"));
	_vectorEnv.push_back((char*)("PATH_NAME=./www/cgi/script.py"));
	_vectorEnv.push_back((char*)("SCRIPT_NAME=./www/cgi/script.py"));
	_vectorEnv.push_back((char*)("SCRIPT_FILENNAME=./www/cgi/script.py"));

	char **env = new char*[3];
	env[0] = (char *)"/usr/bin/python";
	env[1] = (char *)("./www/cgi/script.py");
	env[2] = 0;

	int i = 0;
	char **recupEnv = ft_split(_newIndex.c_str(), '?');
	while (recupEnv[i])
	{
		if (strncmp(recupEnv[i], "f_name", 6) == 0)
			break;
		i++;
	}
	int tmp = open(".tmp", O_CREAT | O_WRONLY | O_TRUNC, 0666);
	write(tmp, recupEnv[i], strlen(recupEnv[i]));
	lseek(tmp, 0, SEEK_SET);
	fd1 = open("emmaCGI", O_CREAT | O_WRONLY | O_TRUNC, 0666);
	std::string len = "CONTENT_LENGTH=" + ws::itos(strlen(recupEnv[i]));
	std::string result = recupEnv[i];
	std::string query = "QUERY_STRING=" + result;
	_vectorEnv.push_back((char *)query.c_str());
	_vectorEnv.push_back((char *)len.c_str());
	_env = ft_regroup_envVector(_vectorEnv);

	int pid = fork();
	if (pid == 0)
	{
		dup2(tmp, 0);
		close(tmp);
		dup2(fd1, 1);
		close(fd1);
		execve(env[0], env, _env);
	}
	else
		wait(NULL);
	freeTab2(recupEnv);
	std::string str1 = ftostr("emmacCGI");
	close(tmp);
	unlink(".tmp");
	delete [] env;
	return (str1);

}


// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

HttpGet &HttpGet::operator=(HttpGet const &rhs) {
	if (this != &rhs) {
		
	}
	return *this;
}


HttpResponse HttpGet::execute(ServerInfo const &info, HttpRequest &request) {
	HttpResponse response;
	DefaultBody *body = new DefaultBody();
	std::ifstream file;
	struct stat fileInfo;
	std::string requestedPath = request.getPath();
	_isCgi = true;

	if (request.getPath()[0] == '/')
		request.setPath(info.getRootPath() + request.getPath());

	if (request.getPath() == info.getRootPath() + "/")
		request.setPath(info.getIndexPath());


	stat(request.getPath().c_str(), &fileInfo);
	file.open(request.getPath().c_str(), std::fstream::ate);

	if (!file.is_open() || !S_ISREG(fileInfo.st_mode)) { // https://stackoverflow.com/a/40163393/10885193
		response.setStatusCode(404);
		std::cout << "REQUEST HAS FAILED" << std::endl;
		body->append(HttpResponse::codes[404].explanation, HttpResponse::codes[404].explanation.size());
		response.addHeader("Content-Type", "text/plain");
		response.addHeader("Content-Length", ws::itos(body->getBody().size() - 1));
		response.addHeader("Connection", "close");
		response.generateDate();

		file.close();
		response.setMessageBody(body);
		return response;
	}

	if (request.getPath() == info.getIndexPath())
		response.addHeader("Content-Type", "text/html");
	else if (request.getPath() == "index.css")
		response.addHeader("Content-Type", "text/css");
	if (request.getPath() == "favicon.ico")
		response.addHeader("Content-Type", "text/favicon");

	size_t fileSize = file.tellg();
	std::string fileContent = std::string(fileSize, ' ');

	file.seekg(0);
	file.read(&fileContent[0], fileSize);

	body->append(fileContent, fileSize);
	response.addHeader("Content-Length", ws::itos(body->getBody().size() - 1));

	response.setMessageBody(body);
	return response;
}

HttpMethod *HttpGet::clone() {
	return new HttpGet();
}


std::string HttpGet::getName() {
	return "GET";
}





