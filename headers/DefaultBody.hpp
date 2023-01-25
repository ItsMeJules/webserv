#ifndef DEFAULTBODY_HPP
# define DEFAULTBODY_HPP

# include "AMessageBody.hpp"

class DefaultBody : public AMessageBody {
	private:
		std::vector<char> _body;
	public:
		DefaultBody();
		DefaultBody(ADataDecoder *decoder);
		DefaultBody(DefaultBody const &defaultBody);
		~DefaultBody();

        int parse(char *body, int &size);
		void append(std::string str, int size);

		std::vector<char> &getBody();
		std::string getBodyStr();

		DefaultBody &operator=(DefaultBody const &rhs);
};

#endif