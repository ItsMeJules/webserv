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

		virtual const std::string build() const;

		void addHeader(std::string headerTag, std::string value);
		const std::string &getHeader(std::string headerTag) const;
        bool headersHasKey(std::string headerTag) const;
        bool headersContains(std::string headerTag, std::string value) const;
		
		void setMessageBody(IMessageBody *messageBody);
		void setHttpVersion(std::string httpVersion);
		
		const std::string &getHttpVersion() const;
		const std::map<std::string, std::string> getHeaders() const;
		IMessageBody *getMessageBody();

		Message &operator=(Message const &rhs);
};

#endif