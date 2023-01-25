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

// 		############## NESTED CLASS FormDataPart ##############

//		############## PUBLIC FormDataPart ##############

FormDataBody::FormDataPart::FormDataPart() : _parsed(false) {}
FormDataBody::FormDataPart::FormDataPart(FormDataPart const &formDataPart) {}

FormDataBody::FormDataPart::~FormDataPart() {}
				
int FormDataBody::FormDataPart::parse(FormDataBody const &parent) {
	std::string partStr = std::string(_contents.data(), _contents.size());
	size_t boundPos = partStr.find("--" + parent._boundary);

	if (boundPos != std::string::npos) {
		size_t headerstartPos = boundPos + 2 + parent._boundary.size();
		size_t headerEndPos;

		while ((headerEndPos = partStr.find("\r\n")) != std::string::npos) {
			std::string header = partStr.substr(headerstartPos, headerEndPos - headerstartPos);
			std::string headerKey = header.substr(0, header.find(":"));
			std::string headerValue = header.substr(headerKey.size());

			if (headerKey == "Content-Disposition") {
				size_t namePos = headerValue.find("name=") + 6; // skips name="
				std::string nameValue = headerValue.substr(namePos, headerValue.size() - (namePos + 1));

				size_t fileNamePos = headerValue.find("filename=" + 10); // skips filename="
				if (fileNamePos != std::string::npos) {
					_fileKey = nameValue;
					_fileName = headerValue.substr(fileNamePos, headerValue.size() - (fileNamePos + 1));
					size_t endPos = partStr.find("\r\n--" + parent._boundary + "--");
					std::vector<char> &content = _directives[nameValue];

					for (int i = headerEndPos + 4; i < endPos; i++)
						content.push_back(partStr[i]);
					_parsed = true;
					return 1;
				}
				size_t dataEndPos = partStr.substr(headerEndPos + 4).find("\r\n");
				std::vector<char> &content = _directives[nameValue];
				
				for (int i = headerEndPos + 4; i < dataEndPos; i++)
					content.push_back(partStr[i]);
			} else
				_headers.insert(std::make_pair(headerKey, headerValue));
		}
	}
	return 0;
}

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
	FormDataPart part;

	if (!_parts.empty()) {
		FormDataPart lastPart = *(_parts.end() - 1);
		if (!lastPart._parsed)
			part = lastPart;
	}

	_decoder->decodeInto(body, size, part._contents);
	return part.parse(*this);
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
