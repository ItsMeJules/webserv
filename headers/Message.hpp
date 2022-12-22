#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <map>
# include <iostream>

# include "MessageBody.hpp"

class Message {
	protected:
		MessageBody _messageBody;

		std::map<std::string, std::string> _headers;

		std::string _httpVersion;
	public:
		Message();
		Message(std::string httpVersion);
		Message(Message const &message);
		virtual ~Message();

		virtual std::string build();
		void addHeader(std::string headerTag, std::string value);
		bool isChunked();

		void setMessageBody(MessageBody messageBody);
		void setHttpVersion(std::string httpVersion);
		
		std::string getHttpVersion() const;

		Message &operator=(Message const &rhs);
};

#endif