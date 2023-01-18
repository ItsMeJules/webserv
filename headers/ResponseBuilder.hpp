#ifndef RESPONSEBUILDER_HPP
# define RESPONSEBUILDER_HPP

# include "HttpRequest.hpp"
# include "ServerInfo.hpp"
# include "utils.hpp"

class ResponseBuilder {
	private:
		ServerInfo const _serverInfo;
		HttpRequest const _request;
	public:

	public:
		ResponseBuilder();
		ResponseBuilder(ServerInfo const &serverInfo, HttpRequest const &request);
		ResponseBuilder(ResponseBuilder const &responseBuilder);
		~ResponseBuilder();

		int const &getStatusCode() const;
		ws::http_status_t const &getStatusPhrase() const;

		ResponseBuilder &operator=(ResponseBuilder const &rhs);
};

#endif