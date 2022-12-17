#include "Message.hpp"

// ############## CONSTRUCTORS / DESTRUCTORS ##############

Message::Message() {}
Message::Message(std::string firstLine, MessageBody messageBody) : _messageBody(messageBody) {}
Message::Message(Message const &message){
	*this = message;
}

Message::~Message() {

}

// ############## PRIVATE ##############

// ############## PUBLIC ##############

void Message::addHeader(std::string headerTag, std::string value) {
	_headers.insert(std::make_pair(headerTag, value));
}

// ############## GETTERS / SETTERS ##############

void Message::setMessageBody(MessageBody messageBody) {
	_messageBody = messageBody;
}
		
// ############## OPERATORS ##############

Message &Message::operator=(Message const &rhs) {
	if (this != &rhs) {
		_messageBody = rhs._messageBody;
		_inReceive = rhs._inReceive;
		_headers = rhs._headers;
	}
	return *this;
}