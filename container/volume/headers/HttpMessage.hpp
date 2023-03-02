#ifndef MESSAGE_HPP
# define MESSAGE_HPP

# include <map>
# include <iostream>

# include "AMessageBody.hpp"

class HttpMessage {
	protected:
		std::string _httpVersion;
		std::map<std::string, std::string> _headers;
		AMessageBody *_messageBody;
	public:
		HttpMessage();
		HttpMessage(std::string httpVersion);
		HttpMessage(HttpMessage const &message);
		virtual ~HttpMessage();

		virtual const std::string build() const;

		void addHeader(std::string headerTag, std::string value);
		const std::string &getHeader(std::string headerTag) const;
        bool headersHasKey(std::string headerTag) const;
        bool headersContains(std::string headerTag, std::string value) const;
		
		void setMessageBody(AMessageBody *messageBody);
		void setHttpVersion(std::string httpVersion);
		
		const std::string &getHttpVersion() const;
		const std::map<std::string, std::string> getHeaders() const;
		AMessageBody *getMessageBody();

		HttpMessage &operator=(HttpMessage const &rhs);
};

#endif