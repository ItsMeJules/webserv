#ifndef CHUNKEDDATADECODER_HPP
# define CHUNKEDDATADECODER_HPP

# include "ADataDecoder.hpp"

class ChunkedDataDecoder : public ADataDecoder {
	private:
		typedef struct s_chunk {
			int size;
			std::vector<char> content;
		} t_chunk;

		t_chunk _actualChunk;
		size_t _sizeRead;

		void clearActualChunk();
		int readChunkSize(size_t const &endChunkPos);
		bool checkChunkSize(int const &bufSize, int const &read);
	public:
		ChunkedDataDecoder();
		ChunkedDataDecoder(ChunkedDataDecoder const &chunkedDataDecoder);
		~ChunkedDataDecoder();

		int decodeInto(std::vector<char> &vec);

		ADataDecoder *clone();

		ChunkedDataDecoder &operator=(ChunkedDataDecoder const &rhs);
};

#endif