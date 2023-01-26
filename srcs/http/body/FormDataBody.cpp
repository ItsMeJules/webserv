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

// 		############## NESTED CLASS FormDataPart ##############

//		############## PRIVATE FormDataPart ##############

int FormDataBody::FormDataPart::strPos(std::string str) const {
	int i;
	int j;

	for (i = 0; i < _contents.size(); i++) {
		j = 0;
		if (_contents[i] == str[j]) {
			while (j < str.size() && i + j < _contents.size() && _contents[i + j] == str[j])
				j++;
			if (j == str.size())
				return i;
		}
	}
	return -1;
}

//		############## PUBLIC FormDataPart ##############

FormDataBody::FormDataPart::FormDataPart() : _parsed(false) {}
FormDataBody::FormDataPart::FormDataPart(FormDataPart const &formDataPart) { *this = formDataPart; }

FormDataBody::FormDataPart::~FormDataPart() {}

bool FormDataBody::FormDataPart::parse(FormDataBody const &parent, size_t const &partEndPos) {
	std::string headerKey = std::string(_contents.data(), strPos(":"));
	
	size_t headerEndPos = strPos("\r\n");
	std::string headerValue = std::string(_contents.data() + headerKey.size() + 2, headerEndPos - headerKey.size() - 2);

	if (headerKey == "Content-Disposition") {
		size_t namePos = headerValue.find("name=") + 6; // skips name="
		std::string nameValue = headerValue.substr(namePos, headerValue.size() - (namePos + 1));
		size_t fileNamePos = headerValue.find("filename=");

		headerEndPos += 4;
		if (fileNamePos != std::string::npos) {
			fileNamePos += 10; // skips filename="
			_fileKey = nameValue.substr(0, nameValue.find("\""));
			_fileName = headerValue.substr(fileNamePos, headerValue.size() - (fileNamePos + 1));
			std::vector<char> &directiveContent = _directives[_fileKey];

			for (int i = headerEndPos; i < partEndPos; i++)
				directiveContent.push_back(_contents[i]);
			_parsed = true;
			return true;
		} else {
			std::vector<char> &directiveContent = _directives[nameValue];

			for (int i = headerEndPos; i < partEndPos; i++)
				directiveContent.push_back(_contents[i]);
		}
	} else
		_headers.insert(std::make_pair(headerKey, headerValue));
	_parsed = true;
	return false;
}

FormDataBody::FormDataPart &FormDataBody::FormDataPart::operator=(FormDataPart const &rhs) {
	if (this != &rhs) {
		_headers = rhs._headers;
		_directives = rhs._directives;
		_fileKey = rhs._fileKey;
		_fileName = rhs._fileName;
		_contents = rhs._contents;
		_parsed = rhs._parsed;
	}
	return *this;
}

// 		############## END OF NESTED CLASS FormDataPart ##############

// ############## PUBLIC ##############

int FormDataBody::parse(char *body, int &size) {
	FormDataPart *part;
	size_t headerStartPos;
	int partEndPos;
	bool parseRet = false;
	
	int decoded = _decoder->decodeInto(body, size, _tmp);
	if (decoded < 1)
		return 0;
	do {
		if (_tmp.size() < _boundary.size() + 4) // boundary not received
			return 0;
		headerStartPos = _boundary.size() + 4;
		partEndPos = nextCRLFpos(headerStartPos, 3);

		if (partEndPos == -1)
			return 0;

		if (!_parts.empty()) {
			FormDataPart *lastPart = *(_parts.end() - 1);
			if (!lastPart->_parsed)
				part = lastPart;
			else {
				part = new FormDataPart();
				_parts.push_back(part);
			}
		} else {
			part = new FormDataPart();			
			_parts.push_back(part);
		}

		for (int i = headerStartPos; i < partEndPos; i++)
			part->_contents.push_back(_tmp[i]);
		// not efficient operation, i could use an offset and just skip the first values
		_tmp.erase(_tmp.begin(), _tmp.begin() + partEndPos + 2);
		parseRet = part->parse(*this, partEndPos - headerStartPos);
	} while (!parseRet);

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
