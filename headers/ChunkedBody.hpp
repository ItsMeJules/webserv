#ifndef CHUNKEDBODY_HPP
# define CHUNKEDBODY_HPP

# include <vector>

# include "IMessageBody.hpp"

class ChunkedBody : public IMessageBody {
    private:
        typedef struct s_block {
            int size;
            std::string content;
        } t_block;

        std::vector<t_block> _blocks;
    public:
        ChunkedBody();
        ChunkedBody(ChunkedBody const &chunkedBody);
        ~ChunkedBody();

        void append(std::string str);
        void append(std::string str, int size);
//        void parse(std::string body);

        std::string getBody() const;
        int getSize() const;
        ChunkedBody &operator=(ChunkedBody const &rhs);
};

#endif
