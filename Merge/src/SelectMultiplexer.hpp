//
// Created by yannou on 08/02/2026.
//

#ifndef WEBSERV_SELECTMULTIPLEXER_HPP
#define WEBSERV_SELECTMULTIPLEXER_HPP

#include <algorithm>
#include "IIOMultiplexer.hpp"

namespace webserv {
    class SelectMultiplexer : public IIOMultiplexer {
        // std::vector<int> m_fds;
        std::vector<int> m_readyFds;
        fd_set m_readSet;
        //fd_set m_writeSet;// Optional: Add if you handle write readiness separately
        fd_set m_masterSet;
    public:
        SelectMultiplexer() {
            FD_ZERO(&m_readSet);
            //FD_ZERO(&m_writeSet);
            FD_ZERO(&m_masterSet);
        }
        virtual ~SelectMultiplexer(){}
        void addFd(int fd) {
            FD_SET(fd, &m_masterSet);
        }
        void removeFd(int fd) {
            FD_CLR(fd, &m_masterSet);
        }
        int wait(int max_fd) {// Wraps select/poll/epoll_wait
            // 1. Reset sets
            m_readSet = m_masterSet;
            m_readyFds.clear(); // Clear previous results

            // 2. Call select
            // Timeout NULL = wait indefinitely
			timeval* test = new timeval;
			test->tv_sec = 30;
			test->tv_usec = 0;

			int activity = select(max_fd + 1, &m_readSet, NULL, NULL, test);

			delete test; 

            if (activity < 0) return -1; // Error
            if (activity == 0) return 0; // Timeout

            // 3. Scan ONCE here so ServerManager doesn't have to
            for (int i = 0; i <= max_fd; ++i) {
                if (FD_ISSET(i, &m_readSet)) {
                    m_readyFds.push_back(i);
                }
            }
            return activity;
        }
        bool isReadReady(int fd) {
            if (find(m_readyFds.begin(), m_readyFds.end() - 1, fd)
                    != m_readyFds.end())
                return true;
            return false;
        }

        std::vector<int> getReadyFds() const{
            return m_readyFds;
        }
        fd_set getMasterSet() const{
            return m_masterSet;
        }
    };
}
#endif //WEBSERV_SELECTMULTIPLEXER_HPP