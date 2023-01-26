#ifndef CGI_HPP
#define CGI_HPP

# include <map>
# include <vector>
# include <cctype>
# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>


#include "HttpRequest.hpp"
#include "Server.hpp"
#include "utils.hpp"

// class Server;

# define BUFFER_SIZE 100000

class Cgi {
	private:
		std::string	_inputBody;
		std::map<std::string, std::string> _env;
		HttpRequest	_httpRequest;

	public:
		Cgi();
		Cgi	&operator=(Cgi const &src);
		Cgi(HttpRequest &request, Server &server);
		~Cgi();

		std::string	execute(const std::string &_binary);

		void	setInputBody(std::string inputBody);
		char **envToTab(void);

		void	setEnv(std::map<std::string, std::string> env);
};

#endif