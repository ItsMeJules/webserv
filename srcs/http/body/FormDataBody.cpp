# include "FormDataBody.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

FormDataBody::FormDataBody(ADataDecoder *decoder, std::string boundaryHeader) :
	AMessageBody(decoder),
	_boundary(boundaryHeader.substr(boundaryHeader.find("boundary=") + 9))
{}

FormDataBody::FormDataBody() : AMessageBody() {}
FormDataBody::FormDataBody(FormDataBody const &formDataBody) : AMessageBody(*this) { *this = formDataBody; }

FormDataBody::~FormDataBody() {
	for (std::vector<FormDataPart*>::iterator it = _parts.begin(); it != _parts.end(); it++)
		delete *it;
}

// ############## PRIVATE ##############

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

void FormDataBody::FormDataPart::parseHeaders() {
	ws::log(ws::LOG_LVL_ALL, "[FormDataBody] -", "a form data is about to parse it's headers.");
	int headerEndPos;

	while ((headerEndPos = ws::pos_in_vec("\r\n", _contents)) != -1) {
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
				
				ws::log(ws::LOG_LVL_DEBUG, "[FormDataPart] -", "file " + _fileName + " with directive " + _directiveName + " has parsed it's headers.");
			} else
				ws::log(ws::LOG_LVL_DEBUG, "[FormDataPart] -", "directive " + _directiveName + " has parsed it's headers.");

		} else {
			_headers.insert(std::make_pair(headerKey, headerValue));
			ws::log(ws::LOG_LVL_DEBUG, "[FormDataPart] -", "header " + headerKey + " was parsed with value: " + headerValue);
		}
		_contents.erase(_contents.begin(), _contents.begin() + headerEndPos + 2);
	}
	_headersParsed = true;
}

void FormDataBody::FormDataPart::parseBody(FormDataBody &parent) {
	std::vector<char> &body = _directives[_directiveName];
	for (std::vector<char>::size_type i = 0; i < parent._tmp.size(); i++)
		body.push_back(parent._tmp[i]);
	parent._tmp.clear();

	int endChunkPos = ws::pos_in_vec("\r\n--" + parent._boundary , body);
	if (endChunkPos != -1) {
		parent._tmp.insert(parent._tmp.begin(), body.begin() + endChunkPos + 2, body.end());
		// parent._tmp.erase(parent._tmp.begin(), parent._tmp.begin() + (body.size() - endChunkPos + 2)); // This can be done faster with this
		body.resize(endChunkPos);	
		_bodyParsed = true;
		ws::log(ws::LOG_LVL_DEBUG, "[FormDataBody] -", "form data parsed it's whole body.");
	} else
		ws::log(ws::LOG_LVL_DEBUG, "[FormDataBody] -", "form data \"" + _directiveName + "\" parsed some body contents.");
}

std::string FormDataBody::FormDataPart::extractBody() {
	std::string str = std::string(&_directives[_directiveName][0], _directives[_directiveName].size());
	_directives[_directiveName].clear();
	return str;
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
	_decoder->addBuffer(body, size);
	int ret = _decoder->decodeInto(_tmp);

	while (ret >= ws::DECODER_CALL_AGAIN) {
		if (ret == ws::DECODER_PARSE_READY) {
			AMessageBody::writeToFile(_tmp);
			_tmp.clear();
		}
		ret = _decoder->decodeInto(_tmp);
	}

	if (ret == ws::DECODER_STOP) {
		AMessageBody::_tmpOfStream.flush();
		AMessageBody::_tmpOfStream.close();
	}
	return ret;
}

AMessageBody *FormDataBody::clone() {
	return new FormDataBody(*this);
}

FormDataBody::FormDataPart *FormDataBody::readForm() {
	size_t headerStartPos = _boundary.size() + 4;
	
	if (_tmp.empty()) {
		if (!AMessageBody::_tmpIfStream.is_open())
			AMessageBody::_tmpIfStream.open(AMessageBody::_tmpFile.name.c_str());
		if (AMessageBody::_tmpIfStream.peek() == EOF)
			return NULL;

		while (_tmp.size() < headerStartPos)
			AMessageBody::appendFromFile(_tmp, ws::SIZE_READ);
	}

	while (!_tmp.empty()) {
		FormDataPart &part = getNextNeedParsing();

		if (!part._headersParsed) {
			int headersEndPos = ws::pos_in_vec("\r\n\r\n", _tmp);

			if (headersEndPos == -1) {
				delete &part;
				break ;
			}
			
			_parts.push_back(&part);
			for (int i = headerStartPos; i < headersEndPos + 2; i++)
				part._contents.push_back(_tmp[i]);

			// not efficient operation, i could maybe use an offset and just skip the first values
			_tmp.erase(_tmp.begin(), _tmp.begin() + headersEndPos + 4); // skips \r\n\r\n
			part.parseHeaders();
		}
		part.parseBody(*this);
		return &part; // we return the part once we've started parsing the body
	}

	return NULL;
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
	std::string body;
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
		for (std::vector<FormDataPart*>::const_iterator it = rhs._parts.begin(); it != rhs._parts.end(); it++) {
			_parts.push_back(new FormDataPart(**it));
			delete *it;
		}
		_tmp = rhs._tmp;
		_boundary = rhs._boundary;
	}
	AMessageBody::operator=(rhs);
	return *this;
}
