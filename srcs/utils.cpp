#include "utils.hpp"

#include <iostream>

int ws::stoi(std::string const &str) {
	int nbr;
	std::stringstream(str) >> nbr;
	return nbr;
}

unsigned int ws::hextoi(std::string const &str) {
	unsigned int x = 0;
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

char *ws::char_array(std::string const &str, int end, int begin) {
	return (new std::vector<char>(str.data() + begin, str.data() + end))->data();
}

void ws::log(int const &level, std::string const &prefix, std::string const &message, const bool &_errno) {
	const int lvl = ws::LOG_LVL & level;
	std::string color;

	if (ws::LOG_LVL & ws::LOG_LVL_NONE || !lvl)
		return ;

	if (lvl == ws::LOG_LVL_ERROR)
		color = ws::C_RED;
	else if (lvl == ws::LOG_LVL_INFO || lvl == ws::LOG_LVL_ALL)
		color = ws::C_YELLOW;
	else if (lvl == ws::LOG_LVL_SUCCESS)
		color = ws::C_LIME;
	else if (lvl == ws::LOG_LVL_DEBUG)
		color = C_SILVER;

	std::cout << "(LOGLVL: " << level << ") " << ws::C_AQUA << prefix << (prefix.empty() ? "" : " ") << color << message << ws::C_RESET << std::endl;
	if (_errno)
		std::cout << "\n error: " << strerror(errno) << std::endl;
}

static int	ws::check_charset(char c, char current)
{
	if (current == c)
	{
		return (1);
	}
	return (0);
}

static int	ws::words_count(char const *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (check_charset(c, s[i]))
		i++;
	while (s[i])
	{
		if (check_charset(c, s[i]))
			count++;
		while (check_charset(c, s[i]))
			i++;
		i++;
	}
	if (!check_charset(c, s[i]))
		count++;
	return (count);
}

static int	ws::wordlen(char const *s, char c)
{
	int	i;
	int	s_size;

	s_size = 0;
	i = 0;
	while (check_charset(c, s[i]) && s[i])
		i++;
	while (!check_charset(c, s[i]) && s[i])
	{
		s_size++;
		i++;
	}
	return (s_size);
}

static char	*ws::dupword(char const *s, char c, int n)
{
	char	*dup;
	int		i;
	int		j;

	i = 0;
	j = 0;
	dup = (char *)malloc(sizeof(char) * (n + 1));
	if (!dup)
		return (NULL);
	while (check_charset(c, s[i]))
		i++;
	while (!check_charset(c, s[i]) && s[i])
	{
		dup[j] = s[i];
		j++;
		i++;
	}
	dup[j] = 0;
	return (dup);
}

char	**ws::ft_split(char const *s, char c)
{
	char	**str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s)
		return (0);
	str = (char **)malloc(sizeof(char *) * (ws::words_count(s, c) + 1));
	if (!str)
		return (NULL);
	while (s[i])
	{
		if (check_charset(c, s[i]))
			i++;
		else
		{
			str[j] = dupword(s + i, c, wordlen(s + i, c));
			j++;
			i += wordlen(s + i, c);
		}
	}
	str[j] = NULL;
	return (str);
}

