#ifndef REGULARBODY_HPP
# define REGULARBODY_HPP

# include "IMessageBody.hpp"

class RegularBody : public IMessageBody {
    private:
        std::stringstream _body;
	    int _size;
        int _contentLength;
    public:
        RegularBody();
        RegularBody(int contentLength);
        RegularBody(RegularBody const &regularBody);
        ~RegularBody();

        void append(std::string str);
        void append(std::string str, int size);
        int parse(std::string body, std::stringstream &inReceive);
        
        const std::string getBody() const;
        const int &getSize() const;
        RegularBody &operator=(RegularBody const &rhs);
};

#endif