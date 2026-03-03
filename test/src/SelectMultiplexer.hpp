/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SelectMultiplexer.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/19 12:29:44 by yroard            #+#    #+#             */
/*   Updated: 2026/02/19 17:26:30 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_SELECTMULTIPLEXER_HPP
#define WEBSERV_SELECTMULTIPLEXER_HPP

#include <algorithm>
#include "IIOMultiplexer.hpp"
#include "Init.hpp"

namespace webserv {
	class SelectMultiplexer : public IIOMultiplexer {
	private:
		std::vector<int> m_readyReadFds;
		std::vector<int> m_readyWriteFds;
		fd_set m_masterReadSet; // Persistent: All connected clients
		fd_set m_masterWriteSet; // Persistent: Clients with pending data
		// Temporaries for select()
		fd_set m_readSet;
		fd_set m_writeSet; //Wanting to write
		Init init;

	public:
		SelectMultiplexer() : m_readyReadFds(), m_readyWriteFds(){
			FD_ZERO(&m_masterReadSet);
			FD_ZERO(&m_masterWriteSet);
			FD_ZERO(&m_readSet);
			FD_ZERO(&m_writeSet); 
		}

		virtual ~SelectMultiplexer(){}

		void addFd(int fd) {
			FD_SET(fd, &m_masterReadSet);
		}

		void removeFd(int fd) {
			FD_CLR(fd, &m_masterReadSet);
			FD_CLR(fd, &m_masterWriteSet);
		}

		void listenWriting(int fd){
			FD_SET(fd, &m_masterWriteSet);
		}

		void stopListeningWriting(int fd) {
			FD_CLR(fd, &m_masterWriteSet);
		}

		int wait(int max_fd) {
			// 1. Prepare sets for select (Reset from Masters)
			m_readSet = m_masterReadSet;
			m_writeSet = m_masterWriteSet;
			// 2. Setup Timeout (Stack allocation is better than new/delete)
			struct timeval timeout;		
			timeout.tv_sec = init.timeOutLimit;
			timeout.tv_usec = 0;
			// 3. Clear previous results
			m_readyReadFds.clear();
			m_readyWriteFds.clear();
			// 4. Call Select
			int activity = select(max_fd + 1, &m_readSet, &m_writeSet, NULL,
				&timeout);
			if (activity < 0) return -1; // Error
			if (activity == 0) return 0; // Timeout
			// 5. Scan results
			for (int i = 0; i <= max_fd; ++i) {
				// Check Read
				if (FD_ISSET(i, &m_readSet)) {
					m_readyReadFds.push_back(i);
				}
				// Check Write
                if (FD_ISSET(i, &m_writeSet)) {
                    m_readyWriteFds.push_back(i);
                }
			}
			return activity;
		}

		bool isReadReady(int fd) {
			if (find(m_readyReadFds.begin(), m_readyReadFds.end() - 1, fd)
					!= m_readyReadFds.end())
				return true;
			return false;
		}

		std::vector<int> getReadyReadFds() const{
			return m_readyReadFds;
		}

		std::vector<int> getReadyWriteFds() const{
			return m_readyWriteFds;
		}

		fd_set getMasterReadSet() const{
			return m_masterReadSet;
		}
	};
}
#endif //WEBSERV_SELECTMULTIPLEXER_HPP