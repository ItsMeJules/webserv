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
	ws::log(ws::LOG_LVL_INFO, "[HTTP GET] -", "executing get request.");
	HttpResponse response;
	std::ifstream fileStream;
	int isValidCode;

	DefaultBody *body = new DefaultBody();
	ws::request_data_t data = HttpMethod::initRequestData(serverInfo, request);

	fileStream.open(data.requestedPath.c_str());
	std::cout << "requested path    ---" << data.requestedPath << std::endl;
	if ((isValidCode = isValid(fileStream, data)) < 400) {
		std::string responseBody;

		std::cout << "ICI" << data.fileExtension << std::endl;
		if (ws::file_is_dir(data.requestedPath)) {
			std::cout << "PASSE ICI" << std::endl;
			if (data.location.getAutoindex() == 1)
				responseBody = ws::html_list_dir(data.requestedPath, data.clientPath);
			else
				response.generateError(404, serverInfo, *body);
		} else if (serverInfo.getCgis().count(data.fileExtension) != 0) {
			std::cout << "ARRIVE DANS LES CGI" << std::endl;
			Cgi *cgi = new Cgi(serverInfo.getCgis());
			if (cgi->setup(request)) {
				std::cout << "CGI SETUP" << std::endl;
				responseBody = cgi->execute(request, data);
				std::cout << "data file extension = " << data.fileExtension << std::endl;
				data.fileExtension = "html";
			} else
				response.generateError(500, serverInfo, *body);

			delete cgi;
		} else
			responseBody = ws::get_file_contents(fileStream, ws::get_file_size(fileStream));

		body->append(responseBody, responseBody.size());
	} else
		response.generateError(isValidCode, serverInfo, *body);

	std::cout << "au dessus des headers GET" << std::endl;
	response.addHeader("Content-Type", ws::mimeTypeFromExtension(data.fileExtension));
	response.addHeader("Content-Length", ws::itos(body->getBodySize()));
	response.addHeader("Date", response.generateDate());
	response.setMessageBody(body);

	std::cout << "TOUOURS DANS EMMA CGI ???" << std::endl;
	fileStream.close();
	std::cout << "FIN DE GET" << std::endl;
	return response;
}

HttpMethod *HttpGet::clone() {
	return new HttpGet();
}


std::string HttpGet::getName() {
	return "GET";
}
