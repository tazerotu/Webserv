/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 14:09:53 by yroard            #+#    #+#             */
/*   Updated: 2026/02/19 16:27:55 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Responsibility: Represents a single active connection. It holds the Client FD,
// the ParsingRequest object, the Response object, and a pointer to the 
//ServerConfig responsible for this client.
//Why: In your main, you have "Case B: Client Socket" where you declare a bunch
//of variables (req, response, targetRoute, statusCode). 
//All of this logic belongs inside a Client class.

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Socket.hpp"
#include "serverConfig/ServerConfig.hpp"
#include "http/ParsingRequest.hpp"
#include <ctime>

namespace webserv {
	class Client {
	private:
		Socket* 				m_socket;
		const serverConfig::ServerConfig*     m_config;
		// Data management
        http::ParsingRequest 	m_request;
		bool 					m_hasFullRequest;
		std::time_t 			m_last_activity;
		std::string 			m_writeBuffer; // Stores data waiting to be sent
		bool 					m_closeConnection; 	// Flag to mark if we should 
													// close after sending

		
		Client(const Client& other) : m_socket(other.m_socket), 
			m_config(other.m_config), m_hasFullRequest(other.m_hasFullRequest),
			m_last_activity(other.m_last_activity), m_writeBuffer(""), 
			m_closeConnection(false){}
			
		Client operator=(const Client& other){
			if (this != &other){
				m_socket = other.m_socket;
				m_config = other.m_config;
				m_hasFullRequest = other.m_hasFullRequest;
				m_last_activity = other.m_last_activity;
			}
			return *this;
		}
		
		void resetTimeOut(){
			m_last_activity = std::time(NULL);
		}
		
	public:
		Client(Socket* socket, const serverConfig::ServerConfig* config)
			: m_socket(socket), m_config(config), m_hasFullRequest(false), 
			m_last_activity(std::time(NULL)) {}
			
		~Client(){
			delete m_socket;
		}

		int getFd() const{
			return m_socket->getSockFd();
		}

		void setRequestAsFull() {
			m_hasFullRequest = true;
		}

		const serverConfig::ServerConfig* getConfig() const {
			return m_config; 
		}

		int receiveData(){
			std::string chunk;
			int error = 0;
            try {
            	chunk = m_socket->receiveMsg(&error);
                if (chunk.empty()) {
	                if (error == EAGAIN || error == EWOULDBLOCK)
	                	return 1;
                	return 0;
                }
				resetTimeOut();
                m_request.appendData(chunk);
                if (m_hasFullRequest == true
                	&& Init::logLevel == DEBUG) {
                	m_request.printRequest();
                }
			}
			catch (std::exception &e) {
				Logger::messagesFilter(ERR,
					"Error during request parsing: ",
					e.what());
				return -1;
            }
			return 1;
		}

		bool hasRequest() const { 
			return m_hasFullRequest; 
		}

        http::ParsingRequest& getRequest() { 
			return m_request; 
		}

        void sendResponse(const std::string& msg) {
			m_writeBuffer.append(msg);
		}

		int handleWrite() {
			if (m_writeBuffer.empty()) {
				return 0; // Nothing to send
        	}
			ssize_t bytesSent = m_socket->sendData(m_writeBuffer);
			if (bytesSent < 0) {
				// Fatal error (Client disconnected)
				return -1; 
			}
			if (bytesSent > 0) {
				// Remove the sent bytes from the beginning of the buffer
				m_writeBuffer.erase(0, bytesSent);
			}
			// After sending, check if we are done
			if (m_writeBuffer.empty()) {
				// Stop listening for write events
				// You need to tell the multiplexer this!
				return 1; // Return 1 to signal "Buffer Empty"
			}
			return 0;
    	}

		bool hasPendingData() const {
			return !m_writeBuffer.empty();
		}
		
		// Check if we are done sending and need to close
		bool readyToClose() const {
			return m_closeConnection && m_writeBuffer.empty();
		}
		
		void setCloseConnection(bool close) {
			m_closeConnection = close;
		}

		void reset(){
			m_request.reset();
			m_hasFullRequest = false;
		}

		bool isReceivingBody() const {
			return m_request.getExpectedSize() > m_request.getBody().size();
		}
		
		// Helper to check timeout from the Main Loop
		// std::cout << "Last activity : " << m_last_activity << "\ntime : "
		// << std::time(NULL) << "\ntime limit : " << secondsLimit << "\ntime passed : "
		// << secondsPassed << std::endl;
		bool hasTimedOut(int secondsLimit) const {
			int secondsPassed = std::difftime(std::time(NULL),
			m_last_activity);
			return secondsPassed >= secondsLimit;
		}
	};
}

#endif