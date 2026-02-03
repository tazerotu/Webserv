/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:35:08 by yroard            #+#    #+#             */
/*   Updated: 2026/01/15 11:38:12 by yroard           ###   ########.fr       */
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

#define BUF_SIZE 2048 // HTTP requests can be larger

namespace webserv {
	class Socket {
	private:
		int m_fd;
	
	public:
		Socket() : m_fd(-1){}
		explicit Socket(const int fd) : m_fd(fd){}
		Socket(const Socket& other) : m_fd(other.m_fd){
			// CONST_CAST is needed because we are modifying the source object
			// to invalidate its FD.
			const_cast<Socket&>(other).m_fd = -1;
			std::cout << "Copy constructor: Ownership transferred for FD "
				<< m_fd << std::endl;
		}

		Socket& operator=(const Socket& other) {
			if (this != &other) {
				// If we already hold a valid FD,
				// close it before taking a new one
				if (m_fd != -1)
					close(m_fd);
				m_fd = other.m_fd;
				// Steal ownership
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

		static void printReceivedRequest(std::string message) {
			std::cout << "received Request:" << std::endl;
			std::cout << "========== START REQUEST ==========" << std::endl;
			for (unsigned i = 0; i < message.size(); ++i) {
				char c = message[i];
				if (c == '\r')
					std::cout << "\\r";		// Print literally "\r"
				else if (c == '\n')
					std::cout << "\\n\n";	// Print literally "\n"
											//then a real newline
				else if (std::isprint(c))
					std::cout << c;
				else
					std::cout << "."; // Replace non-printables with dot
			}
			std::cout << "\n========== END REQUEST ==========" << std::endl;
		}

		std::string receiveMsg(int * returnErrorValue) {
			char req_buf[BUF_SIZE];
			ssize_t bytes_read = 0;
			std::cout << "receiveMsg, m_fd: " << m_fd << std::endl;
			bytes_read = recv(m_fd, req_buf, BUF_SIZE - 1, 0);
			if (bytes_read > 0) {
				std::cout << "receiveMsg, bytes_read: " << bytes_read
					<< std::endl;
				return std::string(req_buf, bytes_read);
			}
			if (bytes_read == 0) {
				// Connection closed by client
				return "";
			}
			// bytes_read == -1. Check why.
			*returnErrorValue = errno;
			if (*returnErrorValue == EAGAIN) {
				// "No more data right now".
				return "";
			}
			throw webserv::EndpointError::create(
				invalid_msg_reception);
			throw std::runtime_error("Socket read error");
			// Consider adding a timeout or
			// max size check to prevent infinite loops
		}

		void sendData(const std::string& data) {
            send(m_fd, data.c_str(), data.size(), 0);
        }
		
		int getSockFd()const {
			return m_fd;
		}
	};
}

#endif


// class Socket {
// 	private:
// 		int m_fd;
// 		Buffer m_msg;
// 		// Add a pointer or member to store the parsed request
// 		// Using a pointer allows it to be NULL initially
// 		http::ParsingRequest* m_request;

// 		Socket() : m_fd(-1), m_msg(), m_request(NULL) {
// 			std::cout << "default constructor called" << std::endl;
// 		}

// 		explicit Socket(const int fd) : m_fd(fd), m_msg(), m_request(NULL) {
// 			std::cout << "constructor called, m_fd= " << m_fd << std::endl;
// 		}

// 	public:
// 		Socket(const Socket& other) : m_fd(other.m_fd), m_msg(other.m_msg){
// 			// CONST_CAST is needed because we are modifying the source object
// 			// to invalidate its FD.
// 			const_cast<Socket&>(other).m_fd = -1;
// 			if (other.m_request)
// 				m_request = new http::ParsingRequest(*other.m_request); // Requires Copy Ctor in ParsingRequest
// 			else
// 				m_request = NULL;
// 			std::cout << "Copy constructor: Ownership transferred for FD " << m_fd << std::endl;
// 		}

// 		Socket& operator=(const Socket& other) {
// 			if (this != &other) {
// 				// If we already hold a valid FD,
// 				// close it before taking a new one
// 				if (m_fd != -1)
// 					close(m_fd);
// 				m_fd = other.m_fd;
// 				m_msg = other.m_msg;
// 				m_request = other.m_request;
// 				// Steal ownership
// 				const_cast<Socket&>(other).m_fd = -1;
// 			}
// 			return *this;
// 		}

// 		~Socket() {
// 			if (m_fd != -1)
// 				close(m_fd);
// 			if (m_request)
// 				delete m_request; // Free memory
// 		}

// 		static Socket createTcpSocket() {
// 			const int tcp_fd = socket(AF_INET, SOCK_STREAM, 0);
// 			if (tcp_fd == -1) {
// 				throw EndpointError::create(invalid_socket);
// 			}
// 			int opt = 1;
// 			if (setsockopt(tcp_fd, SOL_SOCKET, SO_REUSEADDR, &opt,
// 					sizeof(opt)) == -1) {
// 				close(tcp_fd);
// 				throw EndpointError::create(invalid_socket); // or a specific error
// 			}
// 			return Socket(tcp_fd);
// 		}

// 		void listenSocket(Endpoint endpoint) const {
// 			//first, bind
// 			if (bind(m_fd, static_cast<struct sockaddr *>(endpoint.getSockAddr()),
// 					sizeof(struct sockaddr)) == -1)
// 				throw EndpointError::create(invalid_bind,
// 					strerror(errno));
// 			//define backlog to 10
// 			if (listen(m_fd, 10) == -1)
// 				throw EndpointError::create(invalid_listen,
// 					strerror(errno));
// 		}

// 		Socket acceptSocket() const {
// 			struct sockaddr peer_addr;
// 			socklen_t peer_addr_size = sizeof(struct sockaddr);
// 			int client_sfd = accept(m_fd, &peer_addr, &peer_addr_size);
// 			if (client_sfd == -1)
// 				throw EndpointError::create(invalid_accept,
// 						strerror(errno));
// 			else
// 				fcntl(client_sfd, F_SETFL, O_NONBLOCK);
// 			std::cout << "client accepted (FD: " << client_sfd << ")" << std::endl;
// 			return Socket(client_sfd);
// 		}

// 		static void printReceivedRequest(std::string message) {
// 			std::cout << "received Request:" << std::endl;
// 			std::cout << "========== START REQUEST ==========" << std::endl;
// 			for (unsigned i = 0; i < message.size(); ++i) {
// 				char c = message[i];
// 				if (c == '\r')
// 					std::cout << "\\r"; // Print literally "\r"
// 				else if (c == '\n')
// 					std::cout << "\\n\n"; // Print literally "\n" then a real newline
// 				else if (std::isprint(c))
// 					std::cout << c;
// 				else
// 					std::cout << "."; // Replace non-printables with dot
// 			}
// 			std::cout << "\n========== END REQUEST ==========" << std::endl;
// 		}

// 		ssize_t receiveMsg() {
// 			char req_buf[BUF_SIZE];
// 			ssize_t bytes_read = 0;
// 			std::string message;
// 			unsigned sizeMessage = 0;
// 			std::cout << "receiveMsg, m_fd: " << m_fd << std::endl;
// 			while (true){
// 				bytes_read = recv(m_fd, req_buf, BUF_SIZE - 1, 0);
// 				if (bytes_read > 0) {
// 					std::cout << "receiveMsg, bytes_read: " << bytes_read << std::endl;
// 					message.append(req_buf, bytes_read);
// 					sizeMessage += bytes_read;
// 				}
// 				else if (bytes_read == 0) {
// 					// Connection closed by client
// 					return 0;
// 				}
// 				else {
// 					// bytes_read == -1. Check why.
// 					if (errno == EAGAIN || errno == EWOULDBLOCK) {
// 						// This is NOT an error. It just means "No more data right now".
// 						// Stop the loop and process what we have.
// 						break;
// 					}
// 					throw webserv::EndpointError::create(
// 						invalid_msg_reception);
// 					// Consider adding a timeout or
// 					// max size check to prevent infinite loops
// 				}
// 			}
// 			printReceivedRequest(message);
// 			unsigned index = 0;
// 			for (index = 0; index < sizeMessage; ++index) {
// 				m_msg.push_back(message[index]);
// 			}
// 			return sizeMessage;
// 		}

// 		void parseReceivedMsg() {
// 			// Delete old request if any (handling multiple requests
// 			// on one socket)
// 			if (m_request)
// 				delete m_request;
// 			// Create new request and store it
// 			// We need to change ParsingRequest::parseRequest to return a pointer
// 			// OR use 'new' directly here.
// 			m_request = new http::ParsingRequest(
// 				http::ParsingRequest::parseRequest(m_msg));
// 		}

// 		const http::ParsingRequest& getRequest() const {
// 			if (!m_request)
// 				throw std::runtime_error("No request parsed yet");
// 			return *m_request;
// 		}

// 		// Check if we have a request
// 		bool hasRequest() const {
// 			return (m_request != NULL);
// 		}

// 		int getSockFd()const {
// 			return m_fd;
// 		}
// 	};
