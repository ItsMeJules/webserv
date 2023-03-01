#ifndef ADATADECODER_HPP
# define ADATADECODER_HPP

# include <string>
# include <vector>

# include <stdint.h>
# include <stdlib.h>

# include "utils.hpp"

class ADataDecoder {
	protected:
		std::vector<char> _tmp;

		std::string stringFromTmp(size_t const &size);
	public:
		ADataDecoder();
		ADataDecoder(ADataDecoder const &aDataDecoder);
		virtual ~ADataDecoder();

		virtual int decodeInto(std::vector<char> &vec) = 0;
		void addBuffer(const char *buffer, size_t const &readSize);

		bool hasData();

		virtual ADataDecoder *clone() = 0;

		ADataDecoder &operator=(ADataDecoder const &rhs);
};

#endif