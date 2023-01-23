#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <string>

# include <unistd.h>
# include <sys/time.h>

# include "Message.hpp"
# include "Client.hpp"

class HttpResponse : public Message {
	private:
		int _statusCode;
		std::string _reasonPhrase;

		std::string	_body;
		std::string	_response;
		std::string	_content_length;
		std::string	_date;
		std::string	_header;

	public:
		HttpResponse();
		HttpResponse(std::string httpVersion, int statusCode, std::string reasonPhrase);
		HttpResponse(HttpResponse const &htttpResponse);
		~HttpResponse();

		std::string build();
		void send(Client &client);

		std::string	setDate(void){}

		void	cgi(void);

		HttpResponse &operator=(HttpResponse const &rhs);
};

#endif
