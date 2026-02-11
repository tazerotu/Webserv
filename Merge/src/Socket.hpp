/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:35:08 by yroard            #+#    #+#             */
/*   Updated: 2026/02/11 09:55:55 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "Endpoint.hpp"
#include <sys/types.h>
#include <sys/socket.h>
#include <string>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <fcntl.h>
#include <ctime>
#include "EndPointError.hpp"
#include "Logger.hpp"

#define BUF_SIZE 2048 // HTTP requests can be larger

namespace webserv {
	class Socket {
	private:
		int m_fd;
		std::time_t m_last_activity;
	
	public:
		Socket() : m_fd(-1), m_last_activity(std::time(NULL)){}
		explicit Socket(const int fd) : m_fd(fd),
			m_last_activity(std::time(NULL)){}
		Socket(const Socket& other) : m_fd(other.m_fd),
				m_last_activity(other.m_last_activity){
			const_cast<Socket&>(other).m_fd = -1;
			Logger::MessagesFilter(DEBUG,
				 "Copy constructor: Ownership transferred for FD ",
				 ConvUtils::intToStr(m_fd));
		}

		// If we already hold a valid FD,
		// close it before taking a new one
		Socket& operator=(const Socket& other) {
			if (this != &other) {
				if (m_fd != -1)
					close(m_fd);
				m_fd = other.m_fd;
				const_cast<Socket&>(other).m_fd = -1;
			}
			return *this;
		}
		~Socket() {
			if (m_fd != -1) close(m_fd);
		}

		static Socket createTcpSocket() {
			const int tcp_fd = socket(AF_INET, SOCK_STREAM,
				0);
			if (tcp_fd == -1) {
				throw EndpointError::create(invalid_socket);
			}
			int opt = 1;
			if (setsockopt(tcp_fd, SOL_SOCKET, SO_REUSEADDR,
					&opt, sizeof(opt)) == -1) {
				close(tcp_fd);
				throw EndpointError::create(invalid_socket);
				// or a specific error
			}
			return Socket(tcp_fd);
		}

		void listenSocket(Endpoint endpoint) const {
			//first, bind
			if (bind(m_fd, static_cast<struct sockaddr *>(
					endpoint.getSockAddr()),sizeof(struct sockaddr))
					== -1)
				throw EndpointError::create(invalid_bind,
					strerror(errno));
			//define backlog to 10
			if (listen(m_fd, 10) == -1)
				throw EndpointError::create(invalid_listen,
					strerror(errno));
		}

		Socket acceptSocket() const {
			struct sockaddr peer_addr;
			socklen_t peer_addr_size = sizeof(struct sockaddr);
			int client_sfd = accept(m_fd, &peer_addr, &peer_addr_size);
			if (client_sfd == -1)
				throw EndpointError::create(invalid_accept,
						strerror(errno));
			else
				fcntl(client_sfd, F_SETFL, O_NONBLOCK);
			std::cout << "client accepted (FD: " << client_sfd << ")"
				<< std::endl;
			return Socket(client_sfd);
		}

		std::string receiveMsg(int * returnErrorValue) {
			char req_buf[BUF_SIZE];
			ssize_t bytes_read = 0;
			Logger::MessagesFilter(DEBUG, "receiveMsg, m_fd: ",
				ConvUtils::intToStr(m_fd));
			bytes_read = recv(m_fd, req_buf, BUF_SIZE - 1, 0);
			Logger::MessagesFilter(DEBUG,
				"receiveMsg, bytes_read: ",
				ConvUtils::ssizeToStr(bytes_read));
			if (bytes_read > 0) {
				m_last_activity = std::time(NULL);
				Logger::MessagesFilter(DEBUG, "Hello!!!",
					"\n");
				std::string dataPreview(req_buf);
				Logger::MessagesFilter(DEBUG,
					"Raw data preview: ",
					Logger::filterUnprintable(dataPreview));
				return std::string(req_buf, bytes_read);
			}

			if (bytes_read == 0) {
				// Connection closed by client
				return "";
			}
			// bytes_read == -1. Check why.
			*returnErrorValue = errno;
			Logger::MessagesFilter(ERR,
				"receiveMsg, returnErrorValue: ",
				ConvUtils::intToStr(*returnErrorValue));
			if (*returnErrorValue == EAGAIN) {
				// "No more data right now".
				return "";
			}
			throw webserv::EndpointError::create(
				invalid_msg_reception);
		}

		void sendData(const std::string& data) {
            send(m_fd, data.c_str(), data.size(), 0);
        }
		
		int getSockFd()const {
			return m_fd;
		}

		// Helper to check timeout from the Main Loop
		bool hasTimedOut(int secondsLimit) const {
			int secondsPassed = std::difftime(std::time(NULL),
				m_last_activity);
			return secondsPassed > secondsLimit;
		}
	};
}

#endif
