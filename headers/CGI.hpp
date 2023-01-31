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
		std::string	_query;

	public:
		Cgi();
		Cgi	&operator=(Cgi const &src);
		Cgi(HttpRequest &request);
		~Cgi();

		// void	initEnv(HttpRequest &request, Server &server, ServerInfo &serverInfo);

		// char **envToTab(void);
		// std::string	execute(const std::string &_binary);

		std::map<std::string, std::string>	getEnv() const;
		void	setInputBody(std::string inputBody);

		std::string	executeGet(HttpRequest &request);

		std::string	findQuery(HttpRequest &request);
		const std::string &getQuery() const;
		void	setEnv(std::map<std::string, std::string> env);
		std::string getInputBody() const;
};

std::ostream &			operator<<( std::ostream & o, Cgi const & i );
#endif
