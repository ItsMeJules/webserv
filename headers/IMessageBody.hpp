#ifndef IMESSAGEBODY_HPP
# define IMESSAGEBODY_HPP

# include <string>

# include "utils.hpp"

class IMessageBody {
	public:
		virtual void append(std::string str) = 0;
        virtual void append(std::string str, int size) = 0;
//        virtual void parse(std::string body) = 0;

		virtual std::string getBody() const = 0;
		virtual int getSize() const = 0;
};

#endif