#ifndef RESPONSEBUILDER_HPP
# define RESPONSEBUILDER_HPP

# include "HttpRequest.hpp"
# include "ServerInfo.hpp"
# include "utils.hpp"
# include "FileBody.hpp"

class ResponseBuilder {
	private:
		ServerInfo _serverInfo;
		HttpRequest _request;

		int _statusCode;

		bool const handleResponse();
		
		bool const handleSuccess();
		bool const handleRedirection();
		bool const handleClientError();
		bool const handleServerError();
	public:
		ResponseBuilder();
		ResponseBuilder(ServerInfo const &serverInfo, HttpRequest const &request);
		ResponseBuilder(ResponseBuilder const &responseBuilder);
		~ResponseBuilder();

		int const &getStatusCode() const;

		ResponseBuilder &operator=(ResponseBuilder const &rhs);
};

#endif