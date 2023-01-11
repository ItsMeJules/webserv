#ifndef UTILS_HPP
# define UTILS_HPP

# include <string>
# include <sstream>
# include <algorithm>

namespace ws {
    const std::string WHITE_SPACES = " \t\n\v\f\r";
    const std::string LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const std::string PATH_LETTERS = LETTERS + "/_0123456789";

	// CONVERSIONS
	int stoi(std::string const &str);
	unsigned int hextoi(std::string const &str);
	std::string itos(int const &nbr);

    // STRING MANIPULATION
    std::string &skip_chars(std::string &str, std::string const &toSkip);

	bool string_in_range(std::string const &range, std::string const &str);
}

#endif