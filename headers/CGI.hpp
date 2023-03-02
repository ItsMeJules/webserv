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

class Cgi {
	private:
		std::map<std::string, std::string> _env;
		std::map<std::string, std::string> _cgis;
	public:
		Cgi();
		Cgi	&operator=(Cgi const &src);
		Cgi(std::map<std::string, std::string> const &cgis);
		~Cgi();

		char **generateEnv();

		bool cgiExists(std::string const &path);
		std::string	execute(HttpRequest &request, ws::request_data_t &data);

		std::string	findQuery(HttpRequest &request);
		const std::string &getQuery() const;
		bool setup(HttpRequest const &request);
};

std::ostream &			operator<<( std::ostream & o, Cgi const & i );
#endif
