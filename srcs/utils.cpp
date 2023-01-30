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

int ws::pos_in_vec(std::string const &str, std::vector<char> const &vec) {
	int i;
	int j;

	for (i = 0; i < vec.size(); i++) {
		j = 0;
		if (vec[i] == str[j]) {
			while (j < str.size() && i + j < vec.size() && vec[i + j] == str[j])
				j++;
			if (j == str.size())
				return i;
		}
	}
	return -1;
}

int ws::pos_in_vec_from_end(std::string const &str, std::vector<char> const &vec) {
	int i;
	int j;

	for (i = vec.size() - 1; i >= 0; i--) {
		j = str.size() - 1;
		if (vec[i] == str[j]) {
			while (j > 0 && i - j > 0 && vec[i - (str.size() - 1 - j)] == str[j])
				j--;
			if (j == 0)
				return i - (str.size() - 1 - j);
		}
	}
	return -1;
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

bool ws::file_exists(std::string const &path) {
	struct stat fileInfo;
	return stat(path.c_str(), &fileInfo) == 0;
}

bool ws::file_is_reg(std::string const &path) {
	struct stat fileInfo;
	stat(path.c_str(), &fileInfo);
	return !S_ISREG(fileInfo.st_mode);
}