#include "HttpPost.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpPost::HttpPost() {}
HttpPost::HttpPost(HttpPost const &httpPost) { *this = httpPost; }
HttpPost::~HttpPost() {}

// ############## PRIVATE ############## 

// ############## PUBLIC ##############

HttpResponse HttpPost::execute(ServerInfo const &serverInfo, HttpRequest &request) {
	ws::log(ws::LOG_LVL_INFO, "[HTTP POST] -", "executing post request.");
	FormDataBody *formBody = dynamic_cast<FormDataBody*>(request.getMessageBody());
	HttpResponse response;
	DefaultBody *body = new DefaultBody();

	if (request.getPath()[0] == '/')
		request.setPath(serverInfo.getRootPath());

	if (request.getPath() == serverInfo.getRootPath() + "/")
		request.setPath(serverInfo.getIndexPath());
	
	if (formBody != NULL) {
		request.setPath(request.getPath() + serverInfo.getUploadPath() + "/");
		FormDataBody::FormDataPart *part;
		std::ofstream ofs;
		std::string filePath;
		bool opened = false;
			
		while ((part = formBody->readForm()) != NULL) {
			if (part->_headersParsed) {
				if (part->_directiveName != part->_fileKey)
					continue ;

				filePath = std::string(request.getPath() + part->_fileName.c_str());
				if (!ofs.is_open())
					ofs.open(filePath.c_str(), std::ofstream::binary | std::ofstream::out);

				if (ofs.is_open()) {
					ws::log(ws::LOG_LVL_DEBUG, "[HttpPost] -", "data wrote to " + filePath);
					ofs << part->extractBody();
					opened = true;
				}
			}
		}
		if (opened)
			response.setStatusCode(201);
		else
			response.setStatusCode(500);
		ofs.close();
		// if (ws::file_exists(formBody->getFilePart()->_fileName)) {}

		body->append(HttpResponse::codes[response.getStatusCode()].explanation, HttpResponse::codes[response.getStatusCode()].explanation.size());
		response.addHeader("Content-Type", "text/plain");
		response.addHeader("Content-Length", ws::itos(body->getBodySize()));
		response.setMessageBody(body);
	}
	return response;
}

HttpMethod *HttpPost::clone() {
	return new HttpPost();
}

std::string HttpPost::getName() {
	return "POST";
}

// ############## GETTERS / SETTERS ##############

// ############## OPERATORS ##############

HttpPost &HttpPost::operator=(HttpPost const &rhs) {
	if (this != &rhs) {
	}
	return *this;
}
