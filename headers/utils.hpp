#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <sstream>
# include <algorithm>

namespace ws {

	// CONVERSIONS
	int stoi(std::string const &str);
	unsigned int hextoi(std::string const &str);
	std::string itos(int const &nbr);

    // STRING MANIPULATION
    std::string &skip_chars(std::string &str, std::string const &toSkip);
	bool string_in_range(std::string const &range, std::string const &str, size_t npos = std::string::npos);

    // LOGGING
    void log(int const &level, std::string const &prefix, std::string const &message);
}

#endif