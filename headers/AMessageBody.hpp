#ifndef AMESSAGEBODY_HPP
# define AMESSAGEBODY_HPP

# include <iostream>
# include <vector>
# include <fstream>

# include <unistd.h>

# include "utils.hpp"
# include "Constants.hpp"
# include "ADataDecoder.hpp"

class AMessageBody {
	protected:
		ADataDecoder *_decoder;
		ws::tmp_file_t _tmpFile;

		std::ofstream _tmpOfStream;
		std::ifstream _tmpIfStream;
	public:
		AMessageBody();
		AMessageBody(ADataDecoder *decoder);
		AMessageBody(AMessageBody const &messageBody);
		virtual ~AMessageBody();

        virtual int parse(char *body, int &size) = 0;
		bool writeToFile(std::vector<char> const &vec);
		bool appendFromFile(std::vector<char> &vec, std::vector<char>::size_type size);
		void destroyTmpFile();
		virtual AMessageBody *clone() = 0;

		ADataDecoder *getDataDecoder();
		virtual std::string getBodyStr() = 0;

		void setDataDecoder(ADataDecoder *decoder);

		AMessageBody &operator=(AMessageBody const &rhs);
};

#endif
