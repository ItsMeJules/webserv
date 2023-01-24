#ifndef REGULARBODY_HPP
# define REGULARBODY_HPP

# include "IMessageBody.hpp"

class RegularBody : public IMessageBody {
    private:
        std::string _body;

	    int _size;
        int _contentLength;
    public:
        RegularBody();
        RegularBody(int contentLength);
        RegularBody(RegularBody const &regularBody);
        ~RegularBody();

        void append(std::string str, int size);
        int parse(std::string body, std::stringstream &inReceive, int const &bodySize);
		void truncBody(int pos, int npos);
        
        const std::string &getBody() const;
        const int &getSize() const;

        RegularBody &operator=(RegularBody const &rhs);
};

#endif