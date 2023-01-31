# include "FormDataBody.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

FormDataBody::FormDataBody(ADataDecoder *decoder, std::string boundaryHeader) :
	AMessageBody(decoder),
	_boundary(boundaryHeader.substr(boundaryHeader.find("boundary=") + 9))
{}

FormDataBody::FormDataBody() {}
FormDataBody::FormDataBody(FormDataBody const &formDataBody) { *this = formDataBody; }

FormDataBody::~FormDataBody() {
	for (std::vector<FormDataPart*>::iterator it = _parts.begin(); it != _parts.end(); it++)
		delete *it;
}

// ############## PRIVATE ##############

int FormDataBody::nextCRLFpos(int pos, int nCRLF) const {
	while (nCRLF--) {
		while (pos + 1 < _tmp.size()) { // +1 to check for \r\n 
			pos++;
			if (_tmp[pos - 1] == '\r' && _tmp[pos] == '\n')
				break ;
		}
	}
	if (pos + 1 == _tmp.size()) // not found
		return -1;
	return pos - 1;
}

void FormDataBody::removeLastBoundary() {
	FormDataPart *part = *(_parts.end() - 1);
	std::vector<char> &vec = part->_directives[part->_fileKey];
	vec.erase(vec.end() - (_boundary.size() + 8), vec.end());
}

FormDataBody::FormDataPart &FormDataBody::getNextNeedParsing() {
	FormDataPart *part;

	for (std::vector<FormDataPart*>::iterator it = _parts.begin(); it != _parts.end(); it++) {
		if ((*it)->_headersParsed && !(*it)->_bodyParsed) {
 			part = (*it);
			ws::log(ws::LOG_LVL_ALL, "[FormDataBody] -", "form data " + (*it)->_directiveName + " is about to parse some body part.");
			return *part;
		} else if (!(*it)->_headersParsed)
			return **it;
	}
	ws::log(ws::LOG_LVL_ALL, "[FormDataBody] -", "a new form data was created.");
	part = new FormDataPart();
	return *part;
}

// 		############## NESTED CLASS FormDataPart ##############

//		############## PRIVATE FormDataPart ##############

//		############## PUBLIC FormDataPart ##############

FormDataBody::FormDataPart::FormDataPart() : _headersParsed(false), _bodyParsed(false) {}
FormDataBody::FormDataPart::FormDataPart(FormDataPart const &formDataPart) { *this = formDataPart; }

FormDataBody::FormDataPart::~FormDataPart() {}

bool FormDataBody::FormDataPart::parseHeaders(size_t const &headerEndPos) {
	ws::log(ws::LOG_LVL_ALL, "[FormDataBody] -", "a form data is about to parse it's headers.");
	std::string headerKey = std::string(_contents.data(), ws::pos_in_vec(":", _contents));
	std::string headerValue = std::string(_contents.data() + headerKey.size() + 2, headerEndPos - headerKey.size() - 2);

	if (headerKey == "Content-Disposition") {
		size_t namePos = headerValue.find("name=") + 6; // skips name="
		size_t fileNamePos = headerValue.find("filename=");
		_directiveName = headerValue.substr(namePos,  headerValue.find("\"", namePos) - namePos);
		
		if (fileNamePos != std::string::npos) {
			fileNamePos += 10; // skips filename="
			_fileKey = _directiveName;
			_fileName = headerValue.substr(fileNamePos, headerValue.size() - (fileNamePos + 1));
			
			ws::log(ws::LOG_LVL_DEBUG, "[FormDataPart] -", "file " + _fileName + " with directive " + _directiveName + " was parsed.");
		} else {
			ws::log(ws::LOG_LVL_DEBUG, "[FormDataPart] -", "directive " + _directiveName + " was parsed.");
		}
	} else {
		_headers.insert(std::make_pair(headerKey, headerValue));
		ws::log(ws::LOG_LVL_DEBUG, "[FormDataPart] -", "header " + headerKey + " was parsed with value: " + headerValue);
	}
	_headersParsed = true;
	return true;	
}

bool FormDataBody::FormDataPart::parseBody(FormDataBody &parent, int const &decoderRet) {
	std::vector<char> &body = _directives[_directiveName];
	for (int i = 0; i < parent._tmp.size(); i++)
		body.push_back(parent._tmp[i]);
	ws::log(ws::LOG_LVL_DEBUG, "[FormDataBody] -", "form data \"" + _directiveName + "\" has parsed some body contents.");
	parent._tmp.clear();

	int endChunkPos = ws::pos_in_vec("\r\n--" + parent._boundary , body);
	if (endChunkPos != -1) {
		_bodyParsed = true;
		for (int i = endChunkPos + 2; i < body.size(); i++)
			parent._tmp.push_back(body[i]);
		body.resize(endChunkPos);
		ws::log(ws::LOG_LVL_DEBUG, "[FormDataBody] -", "form data has parsed it's whole body, contents:\n" + std::string(parent._tmp.data(), parent._tmp.size()));
	} else {
		
	}
	return true;
}

FormDataBody::FormDataPart &FormDataBody::FormDataPart::operator=(FormDataPart const &rhs) {
	if (this != &rhs) {
		_headers = rhs._headers;
		_directives = rhs._directives;
		_directiveName = rhs._directiveName;
		_fileKey = rhs._fileKey;
		_fileName = rhs._fileName;
		_contents = rhs._contents;
		_headersParsed = rhs._headersParsed;
		_bodyParsed = rhs._bodyParsed;
	}
	return *this;
}

// 		############## END OF NESTED CLASS FormDataPart ##############

// ############## PUBLIC ##############

int FormDataBody::parse(char *body, int &size) {
	int ret;

	_decoder->addBuffer(body, size);
	do {
		ret = _decoder->decodeInto(_tmp);
		while (!_tmp.empty()) {
			FormDataPart &part = getNextNeedParsing();

			if (!part._headersParsed) { // we at least have to receive all headers before parsing them and the body.
				if (_tmp.size() < _boundary.size() + 4) {// boundary not received
					delete &part;
					break ;
				}

				size_t headerStartPos = _boundary.size() + 4;
				int partEndPos = nextCRLFpos(headerStartPos, 2);

				if (partEndPos == -1) {
					delete &part;
					break ;
				}
				_parts.push_back(&part);

				for (int i = headerStartPos; i < partEndPos - 2; i++)
					part._contents.push_back(_tmp[i]);
				// not efficient operation, i could maybe use an offset and just skip the first values
				_tmp.erase(_tmp.begin(), _tmp.begin() + partEndPos + 2); // skips \r\n
				part.parseHeaders(partEndPos - headerStartPos - 2);
			}
			
			part.parseBody(*this, ret);
		}

	} while (ret == ws::DECODER_CALL_AGAIN || ret == ws::DECODER_PARSE_READY);
	
	return ret == ws::DECODER_STOP;
}

// ############## GETTERS / SETTERS ##############

FormDataBody::FormDataPart *FormDataBody::getFilePart() {
	FormDataPart *part;
	for (std::vector<FormDataPart*>::iterator it = _parts.begin(); it != _parts.end(); it++) {
		part = *it;
		if (!part->_fileKey.empty())
			return part;
	}
	return NULL;
}

std::string FormDataBody::getBodyStr() {
	std::cout << "test1" << std::endl;
	std::string body;
	std::cout << "test" << std::endl;
	for (std::vector<FormDataPart*>::iterator it = _parts.begin(); it != _parts.end(); it++) {
		FormDataPart part = **it;
		
		body.append("--" + _boundary + "\r\n", 0, _boundary.size() + 4);
		body.append("Content-Disposition: form-data; ");
		for (std::map<std::string, std::vector<char> >::iterator dit = part._directives.begin(); dit != part._directives.end(); dit++) {
			if (dit->first == part._fileKey)
				body.append("name=\"" + part._fileKey + "\"; filename=\"" + part._fileName + "\"\r\n");
			else
				body.append("name=\"" + dit->first + "\"\r\n");
		}

		for (std::map<std::string, std::string>::iterator hit = part._headers.begin(); hit != part._headers.end(); hit++)
			body.append(hit->first + ": " + hit->second + "\r\n");
		body.append("\r\n");

		for (std::map<std::string, std::vector<char> >::iterator dit = part._directives.begin(); dit != part._directives.end(); dit++)
			body.append(dit->second.data(), dit->second.size());
		body.append("\r\n");
	}
	body.append("--" + _boundary + "--\r\n", 0, _boundary.size() + 6);
	return body;
}

// ############## OPERATORS ##############

FormDataBody &FormDataBody::operator=(FormDataBody const &rhs) {
	if (this != &rhs) {
		_parts = rhs._parts;
		_tmp = rhs._tmp;
		_boundary = rhs._boundary;
	}
	return *this;
}
