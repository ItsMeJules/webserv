#include "HttpDelete.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpDelete::HttpDelete() {}
HttpDelete::HttpDelete(HttpDelete const &httpDelete) : HttpMethod(*this) { *this = httpDelete; }
HttpDelete::~HttpDelete() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

HttpResponse HttpDelete::execute(ServerInfo const &serverInfo, HttpRequest &request) {
	ws::log(ws::LOG_LVL_INFO, "[HTTP DELETE] -", "executing get request.");
	HttpResponse response;
	ws::http_status_t status;

	DefaultBody *body = new DefaultBody();
	std::string root = serverInfo.getRootPath();
	ws::request_data_t data = HttpMethod::initRequestData(serverInfo, request);

	if (remove(data.requestedPath.c_str()) == 0) {
		response.setStatusCode(200);
		status = HttpResponse::codes[response.getStatusCode()];

		body->append(status.explanation, status.explanation.size());
		response.addHeader("Content-Type", "text/plain");
	} else
		response.generateError(404, serverInfo, *body);

	response.addHeader("Content-Length", ws::itos(body->getBodySize()));
	response.addHeader("Date", response.generateDate());
	response.setMessageBody(body);

	return response;
}

HttpMethod *HttpDelete::clone() {
	return new HttpDelete();
}

std::string HttpDelete::getName() {
	return "DELETE";
}

// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

HttpDelete &HttpDelete::operator=(HttpDelete const &rhs) {
	if (this != &rhs) {

	}
	return *this;
}
