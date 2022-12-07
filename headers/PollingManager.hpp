#ifndef POLLINGMANAGER_HPP
# define POLLINGMANAGER_HPP

# include <iostream>
# include <vector>
# include <sys/epoll.h>
# include <unistd.h>
# include <string.h>

class PollingManager {
	private:
		std::vector<epoll_event> fdsReady_;
		int epollFd_;
		struct epoll_event event_;

		PollingManager(PollingManager const &pollingManager) {}

		PollingManager &operator=(PollingManager const &rhs) {
			return *this;
		}
	public:

		// ############## CONSTRUCTORS / DESTRUCTORS ##############

		PollingManager() {
			this->fdsReady_ = std::vector<epoll_event>(5);
		}

		~PollingManager() {
			this->fdsReady_.clear();
			if (close(this->epollFd_) == -1)
				std::cerr << "Failed to close epoll file descriptor!\n" << std::endl;	
		}

		// ############## PUBLIC ##############

		bool createPollInstance() {
			std::cout << "Creating poll instance" << std::endl;
			this->epollFd_ = epoll_create(10); //Nombre arbitraire (voir man page)
			if (this->epollFd_ == -1) 
				std::cerr << "Failed to create poll instance error: " << strerror(errno) << std::endl;
			else
				std::cout << "epoll created with fd: " << this->epollFd_ << std::endl;
			return this->epollFd_ != -1;
		}

		bool pollFd(int fd, uint32_t event) {
			this->event_.events = event;
			this->event_.data.fd = fd;

			int ret = epoll_ctl(this->epollFd_, EPOLL_CTL_ADD, fd, &event_);
			if (ret == -1)
				std::cerr << "Failed to add fd: " << fd << " to polling list!" << std::endl;
			else {
				std::cerr << "Sucessfully added fd: " << fd << " to polling list!" << std::endl;
			}
			return ret != -1;
		}

		int getReadyFdAmount() {
			int readyFdAmount = epoll_wait(this->epollFd_, &this->fdsReady_[0], this->fdsReady_.size(), -1);
			if (readyFdAmount == -1)
				std::cerr << "epoll_wait failed! error: " << strerror(errno) << std::endl;
			return readyFdAmount;
		}

		bool isConnectionRequest(epoll_event event, int listeningFd) {
			return event.data.fd == listeningFd;
		}

		// ############## GETTERS / SETTERS ##############

		epoll_event getEpollEvent(int count) {
			return this->fdsReady_[count];
		}

		int getEpollFd() const {
			return this->epollFd_;
		} 

};

#endif