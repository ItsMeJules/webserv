#ifndef CHUNKEDBODY_HPP
# define CHUNKEDBODY_HPP

# include <vector>

# include "IMessageBody.hpp"
# include "Constants.hpp"

class ChunkedBody : public IMessageBody {
    private:
        std::string _body;

        int _totalSize;
    public:
        ChunkedBody();
        ChunkedBody(ChunkedBody const &chunkedBody);
        ~ChunkedBody();

        void append(std::string str, int size);
        int parse(std::string body, std::stringstream &inReceive, int const &bodySize);
		void truncBody(int pos, int npos);

        const std::string &getBody() const;
        const int &getSize() const;

        ChunkedBody &operator=(ChunkedBody const &rhs);
};

#endif
