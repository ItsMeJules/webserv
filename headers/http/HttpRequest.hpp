#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

# include <string>

# include "Message.hpp"

class HttpRequest : public Message {
	private:
	public:
		HttpRequest();
		HttpRequest(HttpRequest const &httpRequest);
		~HttpRequest();

		std::string build() const;

		HttpRequest &operator=(HttpRequest const &rhs);
};

#endif
