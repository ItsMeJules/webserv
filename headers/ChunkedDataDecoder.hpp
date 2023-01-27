#ifndef CHUNKEDDATADECODER_HPP
# define CHUNKEDDATADECODER_HPP

# include "ADataDecoder.hpp"

class ChunkedDataDecoder : public ADataDecoder {
	private:
		typedef struct s_chunk {
			int size;
			std::vector<char> content;
		} t_chunk;

		t_chunk _previousChunk;
		t_chunk _actualChunk;
		int _sizeRead;

		void clearActualChunk();
		int readChunkSize(std::string const &buffer, size_t const &endChunkPos);
		bool checkChunkSize(const char *buffer, int const &bufSize, int const &read);
	public:
		ChunkedDataDecoder();
		ChunkedDataDecoder(ChunkedDataDecoder const &chunkedDataDecoder);
		~ChunkedDataDecoder();

		int decodeInto(char *buffer, int size, std::vector<char> &vec);

		ChunkedDataDecoder &operator=(ChunkedDataDecoder const &rhs);
};

#endif