#include "HttpDelete.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpDelete::HttpDelete() {}
HttpDelete::HttpDelete(HttpDelete const &httpDelete) { *this = httpDelete; }
HttpDelete::~HttpDelete() {}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

HttpResponse HttpDelete::execute(ServerInfo const &serverInfo, HttpRequest &request) {
	HttpResponse response;
	DefaultBody *body = new DefaultBody();
	std::string root = serverInfo.getRootPath();
	ws::request_data_t data = HttpMethod::initRequestData(serverInfo, request);

	if (remove(data.requestedPath.c_str()) == 0)
	{
		response.setStatusCode(204);
		body->append(HttpResponse::codes[204].explanation, HttpResponse::codes[204].explanation.size());
	}
	else
	{
		response.setStatusCode(404);
		body->append(HttpResponse::codes[404].explanation, HttpResponse::codes[404].explanation.size());
	}
	response.addHeader("Content-Type", "text/plain");
	response.addHeader("Content-Length", ws::itos(body->getBodySize()));
	response.addHeader("Date", response.generateDate());


	(void)request;
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
