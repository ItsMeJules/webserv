#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <sstream>
# include <algorithm>
# include <vector>

# include <string.h>
# include <errno.h>

# include "Constants.hpp"

namespace ws {

	// HTTP
	typedef struct http_status_s {
		std::string reason;
		std::string explanation;
	} http_status_t;

	// CONVERSIONS
	int stoi(std::string const &str);
	unsigned int hextoi(std::string const &str);
	std::string itos(int const &nbr);

    // STRING MANIPULATION
    std::string &skip_chars(std::string &str, std::string const &toSkip);
	bool string_in_range(std::string const &range, std::string const &str, size_t npos = std::string::npos);
	char *char_array(std::string const &str, int size, int begin = 0);

    // LOGGING
    void log(int const &level, std::string const &prefix, std::string const &message, const bool &_errno = false);
}

#endif