#include "HttpMessage.hpp"
#include "FormDataBody.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

HttpMessage::HttpMessage() : _messageBody(NULL) {}
HttpMessage::HttpMessage(std::string httpVersion) : _httpVersion(httpVersion), _messageBody(NULL) {}
HttpMessage::HttpMessage(HttpMessage const &message) { *this = message; }
HttpMessage::~HttpMessage() {
	delete _messageBody;
}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

const std::string HttpMessage::build() const {
	std::stringstream ss;
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); it++)
		ss << it->first << ": " << it->second << "\r\n";
	ss << "\r\n";
	if (_messageBody != NULL)
		ss << _messageBody->getBodyStr();
    return ss.str();
}

void HttpMessage::addHeader(std::string headerTag, std::string value) {
	_headers.insert(std::make_pair(headerTag, value));
}

const std::string &HttpMessage::getHeader(std::string headerTag) const {
    return _headers.at(headerTag);
}

bool HttpMessage::headersHasKey(std::string headerTag) const {
    return _headers.count(headerTag) == 1;
}

bool HttpMessage::headersContains(std::string headerTag, std::string value) const {
	if (_headers.count(headerTag) == 0)
		return false;
	return _headers.at(headerTag) == value;
}

// ############## GETTERS / SETTERS ##############

void HttpMessage::setMessageBody(AMessageBody *messageBody) {
	_messageBody = messageBody;
}

void HttpMessage::setHttpVersion(std::string httpVersion) {
	_httpVersion = httpVersion;
}

AMessageBody *HttpMessage::getMessageBody() {
	return _messageBody;
}

const std::map<std::string, std::string> HttpMessage::getHeaders() const {
    return _headers;
}

const std::string &HttpMessage::getHttpVersion() const {
	return _httpVersion;
}
		
// ############## OPERATORS ##############

HttpMessage &HttpMessage::operator=(HttpMessage const &rhs) {
	if (this != &rhs) {
		_messageBody = (rhs._messageBody != NULL) ? rhs._messageBody->clone() : NULL;
		_headers = rhs._headers;
		_httpVersion = rhs._httpVersion;
	}
	return *this;
}