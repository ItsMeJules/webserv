#ifndef MESSAGEBODY_HPP
# define MESSAGEBODY_HPP

# include <sstream>

class MessageBody {
	private:
		std::stringstream _body;
		int _size;
	public:
		MessageBody();
		MessageBody(MessageBody const &messageBody);
		~MessageBody();

		void append(std::string str);

		std::string getBody() const;
		int getSize() const;

		MessageBody &operator=(MessageBody const &rhs);
};

#endif