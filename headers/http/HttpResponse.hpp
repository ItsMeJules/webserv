#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <string>

# include "Message.hpp"

class HttpResponse : public Message {
	private:
		
	public:
		HttpResponse();
		HttpResponse(HttpResponse const &htttpResponse);
		~HttpResponse();

		std::string build() const;

		HttpResponse &operator=(HttpResponse const &rhs);
};

#endif