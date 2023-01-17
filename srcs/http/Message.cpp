#include "Message.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Message::Message() : _messageBody(NULL) {}
Message::Message(std::string httpVersion) : _httpVersion(httpVersion), _messageBody(NULL) {}
Message::Message(Message const &message) { *this = message; }
Message::~Message() {
    delete _messageBody;
}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

const std::string Message::build() const {
	std::stringstream ss;
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); it++)
		ss << it->first << ": " << it->second << "\r\n";
	ss << "\r\n";
	if (_messageBody != NULL)
		ss << _messageBody->getBody();
    return ss.str();
}

void Message::addHeader(std::string headerTag, std::string value) {
	_headers.insert(std::make_pair(headerTag, value));
}

const std::string &Message::getHeader(std::string headerTag) const {
    return _headers.at(headerTag);
}

bool Message::headersHasKey(std::string headerTag) const {
    return _headers.count(headerTag) == 1;
}

bool Message::headersContains(std::string headerTag, std::string value) const {
	return _headers.count(headerTag) && _headers.at(headerTag) == value;
}

// ############## GETTERS / SETTERS ##############

void Message::setMessageBody(IMessageBody *messageBody) {
	_messageBody = messageBody;
}

void Message::setHttpVersion(std::string httpVersion) {
	_httpVersion = httpVersion;
}

IMessageBody *Message::getMessageBody() {
	return _messageBody;
}

const std::map<std::string, std::string> Message::getHeaders() const {
    return _headers;
}

const std::string &Message::getHttpVersion() const {
	return _httpVersion;
}

// ############## OPERATORS ##############

Message &Message::operator=(Message const &rhs) {
	if (this != &rhs) {
		_messageBody = rhs._messageBody;
		_headers = rhs._headers;
		_httpVersion = rhs._httpVersion;
	}
	return *this;
}
