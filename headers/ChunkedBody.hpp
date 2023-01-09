#ifndef CHUNKEDBODY_HPP
# define CHUNKEDBODY_HPP

# include <vector>

# include "IMessageBody.hpp"

# define HEX_VALUES "0123456789abcdef"

class ChunkedBody : public IMessageBody {
    private:
        typedef struct s_block {
            int size;
            std::string content;
        } t_block;

        std::vector<t_block> _blocks;
        int _hexSize;
    public:
        ChunkedBody();
        ChunkedBody(ChunkedBody const &chunkedBody);
        ~ChunkedBody();

        void append(std::string str);
        void append(std::string str, int size);
        int parse(std::string body, std::stringstream &inReceive);

        std::string getBody() const;
        int getSize() const;
        ChunkedBody &operator=(ChunkedBody const &rhs);
};

#endif
