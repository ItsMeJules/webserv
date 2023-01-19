#include "ResponseBuilder.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############


ResponseBuilder::ResponseBuilder(ServerInfo const &serverInfo, HttpRequest const &request)
	: _serverInfo(serverInfo), _request(request) {}

ResponseBuilder::ResponseBuilder() {}
ResponseBuilder::ResponseBuilder(ResponseBuilder const &responseBuilder) { *this = responseBuilder; }
ResponseBuilder::~ResponseBuilder() {}

// ############## PRIVATE ##############

bool const ResponseBuilder::handleResponse() {
	FileBody *fileBody = dynamic_cast<FileBody*>(_request.getMessageBody());
		if (fileBody != NULL && fileBody->fileCreated())
	_statusCode = 200;
}

// ############## PUBLIC ##############


int const &ResponseBuilder::getStatusCode() const {
	return _statusCode;
}

// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

ResponseBuilder &ResponseBuilder::operator=(ResponseBuilder const &rhs) {
	if (this != &rhs) {
		
	}
	return *this;
}
