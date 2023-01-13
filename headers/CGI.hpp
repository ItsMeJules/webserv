#ifndef CGI_HPP
#define CGI_HPP

#include "utils.hpp"
# include <map>

#include "HttpRequest.hpp"
#include "Server.hpp"

class Cgi {
	private:
		std::string _binary;
		std::string _target;
		std::map<std::string, std::string> _env;
		HttpRequest	_httpRequest;

	public:
		Cgi();
		Cgi(Server const &server, HttpRequest const &request, std::string binary, std::string target);
		~Cgi() {}

		std::string getBinary() const
		{
			return _binary;
		}

		std::string getTarget() const
		{
			return _target;
		}

		std::string	int_to_string(int i);
};

#endif
