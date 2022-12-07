#ifndef CONNECTION_HPP
# define CONNECTION_HPP

# include <iostream>
# include <netinet/in.h>
# include <string.h>
# include <unistd.h>
# include <errno.h>

class Connection {
	protected:
		int domain_;
		int fd_;

		sockaddr_in address_;
	public:
		Connection();
		Connection(int domain);
		Connection(Connection const &connection);
		virtual ~Connection();

		virtual bool generateFd() = 0;
		void setupAddress();
		bool closeConnection();

		int getDomain() const;
		int getFd() const;
		sockaddr_in &getAddress();

		Connection &operator=(Connection const &rhs);
};

#endif