#ifndef CONSTANTS_H
# define CONSTANTS_H

# include <string>

namespace ws {
	const int LOG_LVL = 1;

	const int LVL_NONE = 0;
	const int LVL_INFO = 1;
	const int LVL_ERROR = 2;
	const int LVL_ALL = 3; 

    const std::string WHITE_SPACES = " \t\n\v\f\r";
    const std::string LOW_LETTERS = "abcdefghijklmnopqrstuvwxyz";
    const std::string UP_LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string LETTERS = LOW_LETTERS + UP_LETTERS;
	const std::string HEX_VALUES = "0123456789abcdef";

    const std::string CONFIG_INFO_LETTERS = LETTERS + '_';
    const std::string CONFIG_PATH_LETTERS = LETTERS + "/_0123456789";

	const int POLL_MAX_EVENTS = 10;
	const int POLL_EVENTS_SIZE = 10;
	const int POLL_WAIT_TIMEOUT = 60 * 1000;

	const int RECV_BUFFER_SIZE = 100;
}

#endif