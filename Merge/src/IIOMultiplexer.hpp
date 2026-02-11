//
// Created by yannou on 08/02/2026.
//

#ifndef WEBSERV_IIOMULTIPLEXER_HPP
#define WEBSERV_IIOMULTIPLEXER_HPP

namespace webserv {
    class IIOMultiplexer {
    public:
        virtual ~IIOMultiplexer() {}
        virtual void addFd(int fd) = 0;
        virtual void removeFd(int fd) = 0;
        virtual int wait(int max_fd) = 0; // Wraps select/poll/epoll_wait
        // Access the results after wait()
        virtual std::vector<int> getReadyFds() const = 0;

    };
}

#endif //WEBSERV_IIOMULTIPLEXER_HPP