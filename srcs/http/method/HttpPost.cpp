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

	// if (request.getPath()[0] == '/')
	// 	request.setPath(serverInfo.getRootPath());

	// if (request.getPath() == serverInfo.getRootPath() + "/")
	// 	request.setPath(serverInfo.getIndexPath());
	
	// if (formBody != NULL) {
	// 	request.setPath(request.getPath() + serverInfo.getUploadPath() + "/");
	// 	FormDataBody::FormDataPart *part = formBody->getFilePart();
	// 	// if (ws::file_exists(formBody->getFilePart()->_fileName)) {}

	// 	std::ofstream ofs;
	// 	ofs.open(std::string(request.getPath() + part->_fileName.c_str()).c_str(), std::ofstream::binary | std::ofstream::out);

	// 	if (ofs.is_open()) {
	// 		ofs << std::string(part->_directives[part->_fileKey].data(), part->_directives[part->_fileKey].size());
	// 		response.setStatusCode(201);
	// 	} else {
	// 		response.setStatusCode(500);
	// 	}
	// 	ofs.close();

		// fileBody->createFile(serverInfo.getRootPath() + serverInfo.getUploadPath());
		// response.setStatusCode(fileBody->fileExists() ? 201 : 500);

		response.setStatusCode(200);
		body->append(HttpResponse::codes[response.getStatusCode()].explanation, HttpResponse::codes[response.getStatusCode()].explanation.size());
		response.addHeader("Content-Type", "text/plain");
		response.addHeader("Content-Length", ws::itos(body->getBodySize()));
		response.setMessageBody(body);
	// }
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
