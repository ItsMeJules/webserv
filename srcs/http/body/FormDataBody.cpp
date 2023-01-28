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
	_parts.push_back(part);
	return *part;
}

int FormDataBody::strPos(std::string str, std::vector<char> const &vec) const {
	int i;
	int j;

	for (i = 0; i < vec.size(); i++) {
		j = 0;
		if (vec[i] == str[j]) {
			while (j < str.size() && i + j < vec.size() && vec[i + j] == str[j])
				j++;
			if (j == str.size())
				return i;
		}
	}
	return -1;
}

// 		############## NESTED CLASS FormDataPart ##############

//		############## PRIVATE FormDataPart ##############

//		############## PUBLIC FormDataPart ##############

FormDataBody::FormDataPart::FormDataPart() : _headersParsed(false), _bodyParsed(false) {}
FormDataBody::FormDataPart::FormDataPart(FormDataPart const &formDataPart) { *this = formDataPart; }

FormDataBody::FormDataPart::~FormDataPart() {}

void FormDataBody::FormDataPart::fillContents(int const &begin, int const &end, std::vector<char> &from) {
	for (int i = begin; i < end; i++)
		_contents.push_back(from[i]);
	// not efficient operation, i could maybe use an offset and just skip the first values
	from.erase(from.begin(), from.begin() + end + 2); // skips \r\n
}

bool FormDataBody::FormDataPart::parseHeaders(FormDataBody const &parent, size_t const &headerEndPos) {
	ws::log(ws::LOG_LVL_ALL, "[FormDataBody] -", "a form data is about to parse it's headers.");
	std::string headerKey = std::string(_contents.data(), parent.strPos(":", _contents));
	std::string headerValue = std::string(_contents.data() + headerKey.size() + 2, headerEndPos - headerKey.size() - 2);

	if (headerKey == "Content-Disposition") {
		size_t namePos = headerValue.find("name=") + 6; // skips name="
		size_t fileNamePos = headerValue.find("filename=");
		_directiveName = headerValue.substr(namePos,  headerValue.find("\"", namePos) - namePos);
		
		if (fileNamePos != std::string::npos) {
			fileNamePos += 10; // skips filename="
			_fileKey = _directiveName.substr(0, _directiveName.find("\""));
			_fileName = headerValue.substr(fileNamePos, headerValue.size() - (fileNamePos + 2));
			
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

FormDataBody::FormDataPart &FormDataBody::FormDataPart::operator=(FormDataPart const &rhs) {
	if (this != &rhs) {
		_headers = rhs._headers;
		_directives = rhs._directives;
		_directiveName = rhs._directiveName;
		_fileName = rhs._fileName;
		_contents = rhs._contents;
		_bodyParsed = rhs._bodyParsed;
	}
	return *this;
}

// 		############## END OF NESTED CLASS FormDataPart ##############

// ############## PUBLIC ##############

int FormDataBody::parse(char *body, int &size) {
	size_t headerStartPos;
	int partEndPos = -1;
	
	int decoded = _decoder->decodeInto(body, size, _tmp);
	if (decoded < 1)
		return 0;
	while (decoded > 1) {
		if (_tmp.size() < _boundary.size() + 4) // boundary not received
			return 0;

		headerStartPos = _boundary.size() + 4;
		FormDataPart &part = getNextNeedParsing();

		if (!part._headersParsed) { // we at least have to receive all headers before parsing them and the body.
			partEndPos = nextCRLFpos(headerStartPos, 2);
			if (partEndPos == -1)
				return 0;
			part.fillContents(headerStartPos, partEndPos - 2, _tmp);
			part.parseHeaders(*this, partEndPos - headerStartPos - 2);
		}

		if (decoded == 3) {
			decoded = 1;
			partEndPos = strPos("--" + _boundary + "--\r\n", _tmp);
		} else
			partEndPos = -1;

		for (int i = 0; i < (partEndPos == -1 ? _tmp.size() : partEndPos); i++)
			part._directives[part._directiveName].push_back(_tmp[i]);
		ws::log(ws::LOG_LVL_DEBUG, "[FormDataBody] -", "form data \"" + part._directiveName + "\" has parsed some body contents.");
		ws::log(ws::LOG_LVL_DEBUG, "[FormDataBody] -", "contents:\n" + std::string(_tmp.data(), _tmp.size()));
		_tmp.clear();
	}
	return decoded;
}

// ############## GETTERS / SETTERS ##############

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
		_parts = rhs._parts;
		_tmp = rhs._tmp;
		_boundary = rhs._boundary;
	}
	return *this;
}
