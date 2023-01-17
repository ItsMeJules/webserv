#ifndef CGI_HPP
#define CGI_HPP

# include <map>
# include <vector>
# include <cctype>
# include <unistd.h>
# include <stdio.h>

#include "HttpRequest.hpp"
#include "Server.hpp"
#include "utils.hpp"

class Cgi {
	private:
		std::string _binary;
		std::string _target;
		std::string	_inputBody;
		std::map<std::string, std::string> _env;
		HttpRequest	_httpRequest;
		char **_arg;

	public:
		Cgi();
		Cgi(Server const &server, HttpRequest const &request, std::string binary, std::string target);
		~Cgi() {}

		std::string getBinary() const;

		std::string getTarget() const;
		int	execute(int clientSocket);

		void	setInputBody(std::string inputBody);

		void	setEnv(std::map<std::string, std::string> env);
};

#endif
