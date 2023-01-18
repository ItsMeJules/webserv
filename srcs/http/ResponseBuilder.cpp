#include "ResponseBuilder.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############


ResponseBuilder::ResponseBuilder(ServerInfo const &serverInfo, HttpRequest const &request)
	: _serverInfo(serverInfo), _request(request) {}

ResponseBuilder::ResponseBuilder() {}
ResponseBuilder::ResponseBuilder(ResponseBuilder const &responseBuilder) { *this = responseBuilder; }
ResponseBuilder::~ResponseBuilder() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############



// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

ResponseBuilder &ResponseBuilder::operator=(ResponseBuilder const &rhs) {
	if (this != &rhs) {
		
	}
	return *this;
}
