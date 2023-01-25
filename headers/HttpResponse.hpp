#ifndef HTTPRESPONSE_HPP
# define HTTPRESPONSE_HPP

# include <string>

# include <unistd.h>
# include <sys/time.h>

# include "utils.hpp"
# include "StatusCode.hpp"
# include "Server.hpp"

class HttpResponse : public Message {
	private:
		int _statusCode;
		std::string _reasonPhrase;
		StatusCode	_status_code;

		std::string	_response;
		std::string	_date;
		std::string	_header;
		std::string _type;

	public:
		HttpResponse();
		HttpResponse(std::string httpVersion, int statusCode, std::string reasonPhrase);
		HttpResponse(HttpResponse const &htttpResponse);
		~HttpResponse();

		std::string build();
		void send(Client &client);
		void	get_response(HttpRequest &request, Server &server);
		void	post_response(HttpRequest &request, Server &server);
		void	showErrorPage();

		//getter setter functions
		// std::string	setDate(void){}
		std::string	getResponse() const;

		HttpResponse &operator=(HttpResponse const &rhs);
};

#endif


// faire une fonction de la reponse avec le header et mettre la date dedans
