#ifndef HTTPGET_HPP
# define HTTPGET_HPP

# include <fstream>
# include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

# include "HttpMethod.hpp"
# include "DefaultBody.hpp"

class Cgi;
// class Server;

class HttpGet : public HttpMethod {
	private:
		bool	_isCgi;
		std::vector<char *> _vectorEnv;
		std::vector<char *> _vectorEnvCpy;
		std::string		_newIndex;
		char **_env;
	public:
		HttpGet();
		HttpGet(HttpGet const &httpGet);
		~HttpGet();

		HttpResponse execute(ServerInfo const &info, HttpRequest &request);
		HttpMethod *clone();

		std::string getName();
		bool	isCgi();
		char	**ft_regroup_envVector(std::vector<char *> vec);
		std::string	executeGet(void);
		std::string execFile(std::string file);
		std::string	firstPage(std::string filePath);
		HttpGet &operator=(HttpGet const &rhs);
};

#endif

