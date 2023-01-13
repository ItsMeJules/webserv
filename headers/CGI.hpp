#ifndef CGI_HPP
#define CGI_HPP

#include "utils.hpp"

class Cgi {
	private:
		std::string _binary;
		std::string _target;
		std::map<std::string, std::string> _env;
		HttpRequest	_httpRequest;

	public:
		Cgi (Server const &server, HttpRequest const &request, std::string binary, std::string target) {}
		~Cgi() {}

		std::string getBinary() const
		{
			return _binary;
		}

		std::string getTarget() const
		{
			return _target;
		}
};

#endif
