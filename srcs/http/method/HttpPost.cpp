#include "HttpPost.hpp"
# include "CGI.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpPost::HttpPost() {}
HttpPost::HttpPost(HttpPost const &httpPost) : HttpMethod(*this) { *this = httpPost; }
HttpPost::~HttpPost() {}

// ############## PRIVATE ##############

bool HttpPost::writePartToFile(FormDataBody::FormDataPart &part, std::string const &filePath, std::ofstream &ofs) {
	if (!ofs.is_open())
		ofs.open(filePath.c_str(), std::ofstream::binary | std::ofstream::out);

	if (ofs.is_open()) {
		ws::log(ws::LOG_LVL_DEBUG, "[HttpPost] -", "data wrote to " + filePath);
		ofs << part.extractBody();
		return true;
	}
	return false;
}

// ############## PUBLIC ##############

HttpResponse HttpPost::execute(ServerInfo const &serverInfo, HttpRequest &request) {
	ws::log(ws::LOG_LVL_INFO, "[HTTP POST] -", "executing post request.");
	HttpResponse response;
	std::ifstream fileStream;
	int isValidCode;


	DefaultBody *body = new DefaultBody();
	ws::request_data_t data = HttpMethod::initRequestData(serverInfo, request);

	fileStream.open(data.requestedPath.c_str());

	if ((isValidCode = isValid(fileStream, request, data)) < 400) {
		std::string responseBody;
		FormDataBody *formBody = dynamic_cast<FormDataBody*>(request.getMessageBody());

		if (serverInfo.getCgis().count(data.fileExtension) != 0) {
			Cgi *cgi = new Cgi(serverInfo.getCgis());
			if (cgi->setup(request) && cgi->cgiExists(serverInfo.getCgis().at(data.fileExtension))) {
				responseBody = cgi->execute(request, data);
				data.fileExtension = "html";
			} else
				response.generateError(500, serverInfo, *body);

			delete cgi;
		} else if (formBody != NULL) {
			FormDataBody::FormDataPart *part;
			bool success = true;
			std::ofstream ofs;

			data.requestedPath += data.location.getUploadPath();

			while ((part = formBody->readForm()) != NULL) {
				if (!part->_headersParsed || part->_directiveName != part->_fileKey)
					continue ;

				if (!writePartToFile(*part, data.requestedPath + "/" + part->_fileName, ofs)) {
					ws::log(ws::LOG_LVL_ERROR, "[HttpPost] -", "error when writing to " + data.requestedPath);

					response.generateError(500, serverInfo, *body);
					success = false;
					break ;
				}
			}

			if (success)
				responseBody = HttpResponse::codes[200].explanation;
			if (ofs.is_open())
				ofs.close();
		}

		body->append(responseBody, responseBody.size());
	} else
		response.generateError(isValidCode, serverInfo, *body);


	response.addHeader("Content-Type", ws::mimeTypeFromExtension(data.fileExtension));
	response.addHeader("Content-Length", ws::itos(body->getBodySize()));
	response.addHeader("Date", response.generateDate());
	response.setMessageBody(body);

	fileStream.close();

	return response;
}

HttpPost &HttpPost::operator=(HttpPost const &rhs) {
	if (this != &rhs) {
	}
	return *this;
}

HttpMethod *HttpPost::clone() {
	return new HttpPost();
}

std::string HttpPost::getName() {
	return "POST";
}
