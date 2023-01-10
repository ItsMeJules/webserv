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
    // https://stackoverflow.com/questions/216823/how-to-trim-an-stdstring?page=1&tab=scoredesc#tab-top
    inline std::string &ltrim(std::string &s);
    inline std::string &rtrim(std::string &s);
    inline std::string &trim(std::string &s);

	bool string_in_range(std::string const &range, std::string const &str);
}

#endif