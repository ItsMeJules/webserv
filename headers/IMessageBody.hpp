#ifndef IMESSAGEBODY_HPP
# define IMESSAGEBODY_HPP

# include <iostream>

# include "utils.hpp"
# include "Constants.hpp"

class IMessageBody {
	public:
		virtual void append(std::string str) = 0;
        virtual void append(std::string str, int size) = 0;
        virtual int parse(std::string body, std::stringstream &inReceive) = 0;
		virtual bool excecute(std::string const &method, std::string const &path, std::map<std::string, std::string> const &headers) = 0;

		virtual const std::string getBody() const = 0;
		virtual const int &getSize() const = 0;
};

#endif