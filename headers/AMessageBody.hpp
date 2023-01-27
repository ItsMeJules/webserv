#ifndef AMESSAGEBODY_HPP
# define AMESSAGEBODY_HPP

# include <iostream>
# include <vector>

# include <unistd.h>

# include "utils.hpp"
# include "Constants.hpp"
# include "ADataDecoder.hpp"

class AMessageBody {
	protected:
		ADataDecoder *_decoder; // think to delete
	public:
		AMessageBody();
		AMessageBody(ADataDecoder *decoder);
		AMessageBody(AMessageBody const &messageBody);
		virtual ~AMessageBody();

        virtual int parse(char *body, int &size) = 0;

		ADataDecoder *getDataDecoder();
		virtual std::string getBodyStr() = 0;

		void setDataDecoder(ADataDecoder *decoder);

		AMessageBody &operator=(AMessageBody const &rhs);
};

#endif