#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <map>

# include "MessageBody.hpp"

class Message {
	protected:
		std::string _firstLine;
		std::map<std::string, std::string> _headers;
		MessageBody _messageBody;
	public:
		Message();
		Message(std::string firstLine, MessageBody messageBody);
		Message(Message const &message);
		virtual ~Message();

		void addHeader(std::string headerTag, std::string value);

		void setMessageBody(MessageBody messageBody);

		Message &operator=(Message const &rhs);
};

#endif