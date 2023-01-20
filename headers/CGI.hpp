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

	public:
		Cgi();
		Cgi(Server const &server, HttpRequest &request, std::string binary, std::string target);
		~Cgi() {}

		std::string getBinary() const;

		std::string getTarget() const;
		std::string	execute(void);

		void	setInputBody(std::string inputBody);
		char **envToTab(void);

		void	setEnv(std::map<std::string, std::string> env);
};

#endif
