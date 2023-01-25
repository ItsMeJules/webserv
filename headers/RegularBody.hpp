#ifndef REGULARBODY_HPP
# define REGULARBODY_HPP

# include "utils.hpp"

class RegularBody : public IMessageBody {
    private:
        std::stringstream _body;
        std::string     _msg;
	    int _size;
        int _contentLength;
    public:
        RegularBody();
        RegularBody(int contentLength);
        RegularBody(RegularBody const &regularBody);
        RegularBody(std::string msg) : _msg(msg) {}
        ~RegularBody();

        void append(std::string str);
        void append(std::string str, int size);
        int parse(std::string body, std::stringstream &inReceive);

        // std::string getBody() const;
        std::string getMessage() const;

        // int getSize() const;
        const std::string getBody() const;
        const int &getSize() const;
        RegularBody &operator=(RegularBody const &rhs);
};

#endif
