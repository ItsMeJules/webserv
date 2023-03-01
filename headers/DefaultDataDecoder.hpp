#ifndef DEFAULTDATADECODER_HPP
# define DEFAULTDATADECODER_HPP

# include "ADataDecoder.hpp"

class DefaultDataDecoder : public ADataDecoder {
	private:
		int _contentLength;
		int _sizeRead;
	public:
		DefaultDataDecoder();
		DefaultDataDecoder(int contentLength);
		DefaultDataDecoder(DefaultDataDecoder const &defaultDataDecoder);
		~DefaultDataDecoder();

		int decodeInto(std::vector<char> &vec);
		
		ADataDecoder *clone();

		DefaultDataDecoder &operator=(DefaultDataDecoder const &rhs);
};

#endif