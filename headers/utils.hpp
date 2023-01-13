#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <sstream>
# include <iostream>

# include "Server.hpp"
# include "ServerSocket.hpp"
# include "HttpRequest.hpp"
# include "HttpResponse.hpp"
# include "EPoll.hpp"
# include "ChunkedBody.hpp"
# include "Client.hpp"
# include "FileBody.hpp"
# include "IMessageBody.hpp"
# include "IPoll.hpp"
# include "Message.hpp"
# include "RegularBody.hpp"
# include "RequestParser.hpp"
# include "Socket.hpp"

namespace ws {
	// CONVERSIONS
	int stoi(std::string const &str);
	unsigned int hextoi(std::string const &str);
	std::string itos(int const &nbr);

	//I DONT KNOW
	bool string_in_range(std::string const &range, std::string const &str);
	std::string int_to_string(int i);
}

#endif
