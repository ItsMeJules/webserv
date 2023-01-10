#ifndef STATUSCODE_HPP
# define STATUSCODE_HPP

# include <vector>
# include <string>

#include "utils.hpp"
#include "RequestParser.hpp"

class StatusCode : public RequestParser {
	private:
		RequestParser	request_parser;
		HttpRequest		http_request;
		int	statusCode;
		int	errorCode;

	public :
		StatusCode();
		StatusCode(StatusCode const &src);
		~StatusCode();

		StatusCode &operator=(StatusCode const &rhs);
		void	recupInfoParserRequest(std::string request);

		int	getStatusCode() const {return statusCode;}
};

#endif
