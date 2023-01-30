#ifndef CGI_HPP
#define CGI_HPP

# include <map>
# include <vector>
# include <cctype>
# include <unistd.h>
# include <stdio.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <string>
# include <iostream>
# include <ostream>


#include "HttpRequest.hpp"
#include "Server.hpp"
#include "utils.hpp"


# define BUFFER_SIZE 100000

class Cgi {
	private:
		std::string	_inputBody;
		std::map<std::string, std::string> _env;
		HttpRequest	_httpRequest;
		std::string	_response;

	public:
		Cgi();
		Cgi	&operator=(Cgi const &src);
		Cgi(HttpRequest &request, Server &server, ServerInfo &ServerInfo);
		~Cgi();

		void	initEnv(HttpRequest &request, Server &server, ServerInfo &serverInfo);

		std::string	execute(const std::string &_binary);

		void	setInputBody(std::string inputBody);
		char **envToTab(void);

		void	setEnv(std::map<std::string, std::string> env);
		std::map<std::string, std::string>	getEnv() const;
		std::string getInputBody() const;
};

std::ostream &			operator<<( std::ostream & o, Cgi const & i );
#endif
