#ifndef DEFAULTDATADECODER_HPP
# define DEFAULTDATADECODER_HPP

# include "ADataDecoder.hpp"

class DefaultDataDecoder : public ADataDecoder {
	private:
		int _contentLength;
	public:
		DefaultDataDecoder();
		DefaultDataDecoder(int contentLength);
		DefaultDataDecoder(DefaultDataDecoder const &defaultDataDecoder);
		~DefaultDataDecoder();

		int decodeInto(char *buffer, int size, std::vector<char> &vec);

		DefaultDataDecoder &operator=(DefaultDataDecoder const &rhs);
};

#endif
