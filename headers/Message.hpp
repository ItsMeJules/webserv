#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <map>
# include <vector>
# include <iostream>

# include "MessageBody.hpp"

class Message {
	protected:
		MessageBody _messageBody;

		std::vector<std::string> _inReceive;
		std::map<std::string, std::string> _headers;
		
		virtual void parseFirstLine(std::string firstLine) = 0;
		virtual void parseHeaders(std::string headers) = 0;
		virtual void parseBody(std::string messageBody) = 0;
	public:
	public:
		Message();
		Message(std::string firstLine, MessageBody messageBody);
		Message(Message const &message);
		virtual ~Message();

		virtual void parse(std::string request) = 0;

		void addHeader(std::string headerTag, std::string value);

		void setMessageBody(MessageBody messageBody);

		Message &operator=(Message const &rhs);
};

#endif