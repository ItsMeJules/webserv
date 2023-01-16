#ifndef STATUSCODE_HPP
# define STATUSCODE_HPP

# include <vector>
# include <string>
# include <map>

#include "utils.hpp"
#include "RequestParser.hpp"
#include "HttpResponse.hpp"
#include "RegularBody.hpp"

class StatusCode : public RequestParser {
	private:
		HttpRequest		http_request;
		std::map<int, std::string> _statusCode;

	public :
		StatusCode();
		StatusCode(std::map<int, std::string> statusCode) : _statusCode(statusCode) {}
		StatusCode(StatusCode const &src) {*this = src;}
		StatusCode(HttpRequest request);
		~StatusCode();

		std::string &operator[](int errorCode);
		StatusCode &operator=(StatusCode const &rhs);
		void	testCreateResponse();
		void	writeResponse();

		HttpResponse	createResponse(int errorCode, RegularBody * body);
};

#endif
