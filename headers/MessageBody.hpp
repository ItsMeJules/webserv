#ifndef MESSAGEBODY_HPP
# define MESSAGEBODY_HPP

# include <sstream>

# include "converter.hpp"

class MessageBody {
	private:
		std::stringstream _body;
		int _size;
	public:
		MessageBody();
		MessageBody(std::string body);
		MessageBody(MessageBody const &messageBody);
		~MessageBody();

		int getSizeWith(std::string str);
		void append(std::string str);

		std::string getBody() const;
		std::string getSizeStr() const;
		int getSize() const;

		MessageBody &operator=(MessageBody const &rhs);
};

#endif