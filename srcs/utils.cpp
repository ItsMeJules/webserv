#include "utils.hpp"

#include <iostream>

int ws::stoi(std::string const &str) {
	int nbr;
	std::stringstream(str) >> nbr;
	return nbr;
}

unsigned int ws::hextoi(std::string const &str) {
	unsigned int x;
	std::stringstream ss;
	ss << std::hex << str;
	ss >> x;
	return x;
}

std::string ws::itos(int const &nbr) {
	std::stringstream ss;
	ss << nbr;
	return ss.str();
}

std::string &ws::skip_chars(std::string &str, std::string const &toSkip) {
    int i = 0;
    while (toSkip.find(str[i]) != std::string::npos)
        i++;
    str.erase(0, i);
    return str;
}

bool ws::string_in_range(std::string const &range, std::string const &str, size_t npos) {
    for (int i = 0; i < str.size() && npos != 0; i++, npos--) {
        if (range.find(str[i]) == std::string::npos)
            return false;
    }
	return true;
}

std::string	ft_to_upper(std::string &str)
{
	for (size_t i = 0; i < str.length(); i++)
		str[i] = std::toupper(str[i]);
	return (str);
}

std::vector<std::string>	ft_split(const std::string &str, const std::string &charset)
{
	std::vector<std::string> result;
	if (str.find(charset) != std::string::npos)
	{
		result.push_back(str.substr(0, str.find(charset)));
		result.push_back(str.substr(str.find(charset) + charset.length(), str.length() - str.find(charset) - charset.length() - 1));
	}
	else
		result.push_back(str);
	return result;
}

std::string	replace(std::string src, std::string search, std::string replace)
{
	for (size_t pos = 0; ; pos += replace.length())
	{
		pos = src.find(search, pos);
		if (pos == std::string::npos)
			break ;
		src.erase(pos, search.length());
		src.insert(pos, replace);
	}
	return (src);
}

void	ft_strcpy(const char *src, char *dest)
{
	size_t	i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
}

void	*ft_memset(void *b, int c, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		((unsigned char *)b)[i] = c;
		i++;
	}
	return (b);
}