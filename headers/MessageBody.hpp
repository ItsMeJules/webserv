#ifndef MESSAGEBODY_HPP
# define MESSAGEBODY_HPP

# include <sstream>

class MessageBody {
	private:
		std::stringstream _body;
		int _size;
	public:
		MessageBody();
		MessageBody(std::string body);
		MessageBody(MessageBody const &messageBody);
		~MessageBody();

		void append(std::string str);

		std::string getBody() const;
		std::string getSizeStr() const;
		int getSize() const;

		MessageBody &operator=(MessageBody const &rhs);
};

#endif