#include "StatusCode.hpp"

StatusCode::StatusCode() {}
StatusCode::~StatusCode(){}

void	StatusCode::recupInfoParserRequest(std::string request)
{
	if (parseRequest(request))
		statusCode = 200;
	else
		statusCode = 400;
}
