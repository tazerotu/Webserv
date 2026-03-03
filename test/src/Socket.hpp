/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:35:08 by yroard            #+#    #+#             */
/*   Updated: 2026/02/19 10:27:03 by yroard           ###   ########.fr       */
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

#include "EndPointError.hpp"
#include "Logger.hpp"

#define BUF_SIZE 2048 // HTTP requests can be larger

namespace webserv {
	class Socket {
	private:
		int m_fd;
	
	public:
		Socket() : m_fd(-1){}
		explicit Socket(const int fd) : m_fd(fd){}
		Socket(const Socket& other) : m_fd(other.m_fd){
			const_cast<Socket&>(other).m_fd = -1;
			Logger::messagesFilter(DEBUG,
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
			}
			return Socket(tcp_fd);
		}

		void listenSocket(Endpoint endpoint) const {
			if (bind(m_fd, static_cast<struct sockaddr *>(
					endpoint.getSockAddr()),sizeof(struct sockaddr))
					== -1)
				throw EndpointError::create(invalid_bind,
					strerror(errno));
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
			Logger::messagesFilter(DEBUG, "receiveMsg, m_fd: ",
				ConvUtils::intToStr(m_fd));
			bytes_read = recv(m_fd, req_buf, BUF_SIZE - 1, 0);
			Logger::messagesFilter(DEBUG,
				"receiveMsg, bytes_read: ",
				ConvUtils::ssizeToStr(bytes_read));
			if (bytes_read > 0) {
				std::string dataPreview(req_buf);
				Logger::messagesFilter(DEBUG,
					"Raw data preview: ",
					Logger::filterUnprintable(dataPreview));
				return std::string(req_buf, bytes_read);
			}
			if (bytes_read == 0) {
				// Connection closed by client
				return "";
			}
			*returnErrorValue = errno;
			Logger::messagesFilter(ERR,
				"receiveMsg, returnErrorValue: ",
				std::strerror(errno));
			if (*returnErrorValue == EAGAIN) {
				return "";
			}
			throw webserv::EndpointError::create(
				invalid_msg_reception);
		}

		ssize_t sendData(const std::string& data) {
			if (data.empty())
				return 0;
			errno = 0;
            ssize_t bytes_sent = send(m_fd, data.c_str(), data.size(), MSG_NOSIGNAL);
			if (bytes_sent == -1) {
				// If the kernel buffer is full, it's not a fatal error. 
				// We just can't send right now.
				if (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR) {
					return 0; 
				}
				Logger::messagesFilter(ERR,
				"client disconnected during sending data, ErrorValue: ",
				std::strerror(errno));
				return -1; 
			}
			return bytes_sent;
        }
		
		int getSockFd()const {
			return m_fd;
		}
	};
}

#endif