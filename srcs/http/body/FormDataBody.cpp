# include "FormDataBody.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

FormDataBody::FormDataBody(ADataDecoder *decoder, std::string boundaryHeader) :
	AMessageBody(decoder),
	_boundary(boundaryHeader.substr(boundaryHeader.find("boundary=") + 9))
{}

FormDataBody::FormDataBody() {}
FormDataBody::FormDataBody(FormDataBody const &formDataBody) { *this = formDataBody; }

FormDataBody::~FormDataBody() {}

// ############## PRIVATE ##############

void FormDataBody::printVector() const {
	for (std::vector<char>::const_iterator it = _tmp.begin(); it != _tmp.end(); it++)
		std::cout << *it;
	std::cout << std::endl;
}

int FormDataBody::nextCRLFpos(int pos, int nCRLF) const {
	while (nCRLF--) {
		while (pos + 1 < _tmp.size()) { // +1 to check for \r\n 
			pos++;
			if (_tmp[pos - 1] == '\r' && _tmp[pos] == '\n')
				break ;
		}
	}
	if (pos == _tmp.size()) // not found
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
FormDataBody::FormDataPart::FormDataPart(FormDataPart const &formDataPart) {}

FormDataBody::FormDataPart::~FormDataPart() {}

int FormDataBody::FormDataPart::parse(FormDataBody const &parent, size_t const &partEndPos) {
	std::string headerKey = std::string(_contents.data(), strPos(":"));
	
	size_t headerEndPos = strPos("\r\n") - 1;
	std::string headerValue = std::string(_contents.data() + headerKey.size() + 2, headerEndPos - headerKey.size() - 1);

	if (headerKey == "Content-Disposition") {
		size_t namePos = headerValue.find("name=") + 6; // skips name="
		std::string nameValue = headerValue.substr(namePos, headerValue.size() - (namePos + 1));

		size_t fileNamePos = headerValue.find("filename=");
		if (fileNamePos != std::string::npos) {
			fileNamePos += 10; // skips filename="
			_fileKey = nameValue;
			_fileName = headerValue.substr(fileNamePos, headerValue.size() - (fileNamePos + 1));
			std::vector<char> &content = _directives[nameValue];

			for (int i = headerEndPos; i < partEndPos; i++) {
				std::cout << _contents[i];
				content.push_back(_contents[i]);
			}
			_parsed = true;
			return 1;
		}
		std::vector<char> &directiveContent = _directives[nameValue];
		std::cout << headerEndPos << " " << partEndPos << std::endl;
		for (int i = headerEndPos + 2; i < partEndPos; i++) {
			std::cout << _contents[i];
			directiveContent.push_back(_contents[i]);
		}
	} else
		_headers.insert(std::make_pair(headerKey, headerValue));

	return 1;
}
				
// int FormDataBody::FormDataPart::parse(FormDataBody const &parent) {
// 	std::string partStr = std::string(_contents.data(), _contents.size());
// 	size_t boundPos = partStr.find("--" + parent._boundary);

// 	if (boundPos != std::string::npos) {
// 		size_t headerStartPos = boundPos + 2 + parent._boundary.size() + 2;
// 		size_t headerEndPos = partStr.find("\r\n");

// 		partStr.erase(0, headerStartPos);
// 		std::cout << partStr << std::endl;
// 		std::cout << headerEndPos << " " << partStr.size() << std::endl;
// 		std::string header = partStr.substr(0, headerEndPos);
// 		std::cout << header << std::endl;
// 		std::string headerKey = header.substr(0, header.find(":"));
// 		std::cout << headerKey << std::endl;
// 		std::string headerValue = header.substr(headerKey.size() + 2);
// 		std::cout << headerValue << std::endl;
		
// 		if (headerKey == "Content-Disposition") {
// 			size_t namePos = headerValue.find("name=") + 6; // skips name="
// 			std::string nameValue = headerValue.substr(namePos, headerValue.size() - (namePos + 1));

// 			headerEndPos += 4;
// 			size_t fileNamePos = headerValue.find("filename=");
// 			if (fileNamePos != std::string::npos) {
// 				fileNamePos += 10; // skips filename="
// 				_fileKey = nameValue;
// 				_fileName = headerValue.substr(fileNamePos, headerValue.size() - (fileNamePos + 1));
// 				size_t endPos = partStr.find("\r\n--" + parent._boundary + "--");
// 				std::vector<char> &content = _directives[nameValue];

// 				for (int i = headerEndPos; i < endPos; i++)
// 					content.push_back(partStr[i]);
// 				_parsed = true;
// 				return 1;
// 			}
// 			size_t dataEndPos = partStr.substr(headerEndPos).find("\r\n") + headerEndPos;
// 			std::cout << nameValue << std::endl;
// 			std::vector<char> &content = _directives[nameValue];
			
// 			for (int i = headerEndPos; i < dataEndPos; i++)
// 				content.push_back(partStr[i]);
// 			_contents.erase(_contents.begin(), _contents.begin() + (dataEndPos + 2) * sizeof(std::vector<char>::size_type));
// 			parse(parent);
// 		} else
// 			_headers.insert(std::make_pair(headerKey, headerValue));
// 	}
// 	return 0;
// }

FormDataBody::FormDataPart &FormDataBody::FormDataPart::operator=(FormDataPart const &rhs) {
	if (this != &rhs) {
		_headers = rhs._headers;
		_directives = rhs._directives;
		_contents = rhs._contents;
	}
	return *this;
}

// 		############## END OF NESTED CLASS FormDataPart ##############

// ############## PUBLIC ##############

int FormDataBody::parse(char *body, int &size) {
	_decoder->decodeInto(body, size, _tmp);

	if (_tmp.size() < _boundary.size() + 4) // boundary not received
		return 0;
	
	size_t headerStartPos = _boundary.size() + 4;
	size_t partEndPos = nextCRLFpos(headerStartPos, 3);

	FormDataPart part;
	if (!_parts.empty()) {
		FormDataPart lastPart = *(_parts.end() - 1);
		if (!lastPart._parsed)
			part = lastPart;
	}

	for (int i = headerStartPos; i < partEndPos; i++)
		part._contents.push_back(_tmp[i]);
	int ret = part.parse(*this, partEndPos - headerStartPos);

	// for (std::vector<FormDataPart>::iterator it = _parts.begin(); it != _parts.end(); it++)
	// 	std::cout << std::string(it->_contents.data(), it->_contents.size()) << std::endl;
	return ret;
}

// ############## GETTERS / SETTERS ##############

std::string FormDataBody::getBodyStr() {
	std::string body;

	for (std::vector<FormDataPart>::iterator it = _parts.begin(); it != _parts.end(); it++)
		body.append(it->_contents.data(), it->_contents.size());
	return body;
}


// ############## OPERATORS ##############

FormDataBody &FormDataBody::operator=(FormDataBody const &rhs) {
	if (this != &rhs) {
		_parts = rhs._parts;
		_boundary = rhs._boundary;
	}
	return *this;
}
