#include "HttpGet.hpp"
#include "CGI.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpGet::HttpGet() {}
HttpGet::HttpGet(HttpGet const &httpGet) : HttpMethod(*this) { *this = httpGet; }
HttpGet::~HttpGet() {}


HttpGet &HttpGet::operator=(HttpGet const &rhs) {
	if (this != &rhs) {
	}
	return *this;
}

HttpResponse HttpGet::execute(ServerInfo const &serverInfo, HttpRequest &request) {
	HttpResponse response;
	std::ifstream fileStream;
	size_t fileSize;

	DefaultBody *body = new DefaultBody();
	ws::request_data_t data = HttpMethod::initRequestData(serverInfo, request);

	fileStream.open(data.requestedPath.c_str());

	if (!isValid(fileStream, data, serverInfo, body, response)) {
		response.addHeader("Content-Length", ws::itos(body->getBodySize()));
		response.addHeader("Date", response.generateDate());
		response.setMessageBody(body);
		return response;
	}

	if (ws::file_is_dir(data.requestedPath)) {
		if (data.location.getAutoindex() == 1) {	
			std::string autoIndexStr = ws::html_list_dir(data.requestedPath, data.clientPath);
			body->append(autoIndexStr, autoIndexStr.size());
		} else
			response.generateError(404, serverInfo, *body);
		response.addHeader("Content-Length", ws::itos(body->getBodySize()));
		response.addHeader("Date", response.generateDate());
		response.setMessageBody(body);

		return response;
	}

	if (serverInfo.getCgis().count(data.fileExtension) != 0) {
		Cgi *cgi = new Cgi(serverInfo.getCgis());

		if (cgi->setup(request)) {
			std::string responseReturn = cgi->execute(request, data);
			body->append(responseReturn, responseReturn.size());
		} else
			response.generateError(500, serverInfo, *body);

		std::cerr << "********************CGI***********************" << std::endl;

		response.addHeader("Content-Length", ws::itos(body->getBodySize()));
		response.addHeader("Date", response.generateDate());
		response.setMessageBody(body);

		delete cgi;
		return response;
	}

	fileSize = ws::get_file_size(fileStream);
	body->append(ws::get_file_contents(fileStream, fileSize), fileSize);

	response.addHeader("Content-Type", ws::mimeTypeFromExtension(data.fileExtension));
	response.addHeader("Content-Length", ws::itos(body->getBodySize()));
	response.addHeader("Date", response.generateDate());
	response.setMessageBody(body);

	fileStream.close();
	return response;
}

HttpMethod *HttpGet::clone() {
	return new HttpGet();
}


std::string HttpGet::getName() {
	return "GET";
}
