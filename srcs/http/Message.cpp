#include "Message.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Message::Message() {}
Message::Message(std::string httpVersion) : _httpVersion(httpVersion) {}
Message::Message(Message const &message) {
	*this = message;
}

Message::~Message() {

}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

std::string Message::build() {
	std::stringstream ss;
	for (std::map<std::string, std::string>::const_iterator it = _headers.begin(); it != _headers.end(); it++)
		ss << it->first << ": " << it->second << "\r\n";
	ss << "\r\n";
	ss << _messageBody.getBody();
	return ss.str();
}

void Message::addHeader(std::string headerTag, std::string value) {
	_headers.insert(std::make_pair(headerTag, value));
}

bool Message::isChunked() {
	return _headers.count("Transfer-Encoding") && _headers["Transfer-Encoding"] == "chunked";
}

// ############## GETTERS / SETTERS ##############

void Message::setMessageBody(MessageBody messageBody) {
	_messageBody = messageBody;
}

void Message::setHttpVersion(std::string httpVersion) {
	_httpVersion = httpVersion;
}

std::string Message::getHttpVersion() const {
	return _httpVersion;
}
		
// ############## OPERATORS ##############

Message &Message::operator=(Message const &rhs) {
	if (this != &rhs) {
		_messageBody = rhs._messageBody;
		_headers = rhs._headers;
	}
	return *this;
}