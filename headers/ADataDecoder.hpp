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

		void fillTmp(char *buffer, size_t const &size);
		std::string bufferWithTmp(char *buffer, size_t const &size);
	public:
		ADataDecoder();
		ADataDecoder(ADataDecoder const &aDataDecoder);
		virtual ~ADataDecoder();

		virtual int decodeInto(char *buffer, int size, std::vector<char> &vec) = 0;

		ADataDecoder &operator=(ADataDecoder const &rhs);
};

#endif