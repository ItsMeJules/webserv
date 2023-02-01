#ifndef CONSTANTS_H
# define CONSTANTS_H

# include <string>

namespace ws {

	const int LOG_LVL_NONE = 1; //		00000001
	const int LOG_LVL_INFO = 2; //		00000010
	const int LOG_LVL_SUCCESS = 4; // 	00000100
	const int LOG_LVL_ERROR = 8; //		00001000
	const int LOG_LVL_ALL = 16; //		00010000
	const int LOG_LVL_DEBUG = 32; //	00100000
	const int LOL_LVL_PARSING = 64; //  01000000
	// 00111110
	const int LOG_LVL = LOG_LVL_INFO | LOG_LVL_SUCCESS | LOG_LVL_ERROR | LOG_LVL_ALL | LOG_LVL_DEBUG;

	const std::string SPACE = " ";
	const std::string WHITE_SPACES = " \t\n\v\f\r";
    const std::string LOW_LETTERS = "abcdefghijklmnopqrstuvwxyz";
    const std::string UP_LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const std::string LETTERS = LOW_LETTERS + UP_LETTERS;
	const std::string HEX_VALUES = "0123456789abcdef";
	const std::string AL_NUM = LETTERS + "0123456789";

    const std::string CONFIG_INFO_LETTERS = LETTERS + '_';
    const std::string CONFIG_PATH_LETTERS = LETTERS + "./_0123456789";

	const int POLL_MAX_EVENTS = 10;
	const int POLL_EVENTS_SIZE = 10;
	const int POLL_WAIT_TIMEOUT = 60 * 1000;

	const int GIGA = 1000000000;
	const int MEGA = 1000000;
	const int KILO = 1000;

	const int RECV_BUFFER_SIZE = 10000;

	const std::string C_AQUA = "\033[38;5;14m";
	const std::string C_YELLOW = "\033[38;5;184m";
	const std::string C_LIME = "\033[38;5;10m";
	const std::string C_RED	= "\033[38;5;196m";
	const std::string C_SILVER = "\033[38;5;7m";
	const std::string C_RESET = "\033[0m";

	//ADataDecoder
	const int DECODER_STOP = 0;
	const int DECODER_WAITING_FOR_RECV = 1;
	const int DECODER_CALL_AGAIN = 2;
	const int DECODER_PARSE_READY = 3;

	//ChunkedDataDecoder
	const int DECODER_CHUNKED_NOT_HEX = -1;
	const int DECODER_CHUNKED_CHUNK_TOO_BIG = -2;

	// tmp files
	const std::string TMP_PATH = "/mnt/nfs/homes/jpeyron/Documents/webserv/tmp";
	const int SIZE_READ = 8192;
}

#endif
