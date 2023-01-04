#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <map>
# include <iostream>

# include "IMessageBody.hpp"

class Message {
	protected:
		std::string _httpVersion;
		std::map<std::string, std::string> _headers;
		IMessageBody *_messageBody;
	public:
		Message();
		Message(std::string httpVersion);
		Message(Message const &message);
		virtual ~Message();

		virtual std::string build();
		void addHeader(std::string headerTag, std::string value);
		std::string getHeader(std::string headerTag);
		bool headersContains(std::string headerTag, std::string value);
		
		void setMessageBody(IMessageBody *messageBody);
		void setHttpVersion(std::string httpVersion);
		
		std::string getHttpVersion() const;
		IMessageBody *getMessageBody();

		Message &operator=(Message const &rhs);
};

#endif