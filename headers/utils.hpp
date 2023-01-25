#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <sstream>
# include <iostream>
# include <vector>
# include <map>

# include <algorithm>

# include <string.h>
# include <errno.h>

# include "Constants.hpp"
# include "Client.hpp"
# include "ClientSocket.hpp"
# include "CGI.hpp"
# include "ChunkedBody.hpp"
# include "config_parser.hpp"
# include "EPoll.hpp"
# include "FileBody.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "IMessageBody.hpp"
# include "IPoll.hpp"
# include "Message.hpp"
# include "Poll.hpp"
# include "RegularBody.hpp"
# include "RequestParser.hpp"
# include "Server.hpp"
# include "Socket.hpp"
# include "ServerInfo.hpp"
# include "ServerSocket.hpp"
# include "StatusCode.hpp"


namespace ws {

	// CONVERSIONS
	int stoi(std::string const &str);
	unsigned int hextoi(std::string const &str);
	std::string itos(int const &nbr);

	//I DONT KNOW
	std::string					ft_to_upper(std::string &str);



    // STRING MANIPULATION
    std::string &skip_chars(std::string &str, std::string const &toSkip);
	bool string_in_range(std::string const &range, std::string const &str, size_t npos = std::string::npos);
	void	ft_strcpy(const char *src, char *dest);

    // LOGGING
    void log(int const &level, std::string const &prefix, std::string const &message, const bool &_errno = false);
}
	std::string					replace(std::string src, std::string search, std::string replace);
	std::vector<std::string> ft_split(const std::string &str, const std::string &charset);
	std::string	ft_to_upper(std::string &str);
	void	*ft_memset(void *b, int c, size_t len);
	void	ft_strcpy(const char *src, char *dest);




#endif
