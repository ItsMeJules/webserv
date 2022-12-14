#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <map>

# include "MessageBody.hpp"

class Message {
	protected:
		std::string _firstLine;
		std::map<std::string, std::string> _headers;
		MessageBody _messageBody;

		virtual void parseFirstLine(std::string firstLine) = 0;
		virtual void parseHeaders(std::string headers) = 0;
		virtual void parseBody(std::string messageBody) = 0;
	public:
		Message();
		Message(std::string firstLine, MessageBody messageBody);
		Message(Message const &message);
		virtual ~Message();

		virtual void parse(std::string &request) = 0;

		void addHeader(std::string headerTag, std::string value);

		void setMessageBody(MessageBody messageBody);

		Message &operator=(Message const &rhs);
};

#endif