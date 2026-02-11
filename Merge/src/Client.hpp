/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 14:09:53 by yroard            #+#    #+#             */
/*   Updated: 2026/02/11 13:14:27 by ttas             ###   ########.fr       */
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
		Socket* 				m_socket;	// The mechanism
		const serverConfig::ServerConfig*     m_config;   // The rules
		// Data management
        http::ParsingRequest 	m_request;
		bool 					m_hasFullRequest;
		std::time_t 			m_last_activity;
		Client(const Client& other) : m_socket(other.m_socket), 
			m_config(other.m_config), m_hasFullRequest(other.m_hasFullRequest),
			m_last_activity(other.m_last_activity){}
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
	// Client takes ownership of the Socket pointer
		Client(Socket* socket, const serverConfig::ServerConfig* config)
			: m_socket(socket), m_config(config), m_hasFullRequest(false), 
			m_last_activity(std::time(NULL)) {}
			
		~Client(){
			delete m_socket; // Client destroys the connection when it dies
		}

		// Returns the FD
		int getFd() const{
			return m_socket->getSockFd();
		}

		void setRequestAsFull() {
			m_hasFullRequest = true;
		}

		const serverConfig::ServerConfig* getConfig() const {
			return m_config; 
		}

		// Reads data from socket into request parser
		// Returns bytes read (0 = disconnected, -1 = error)
		int receiveData(){
			// 1. Use Socket to get bytes
			std::string chunk;
			int error = 0;
            try {
				// 1. Network Responsibility: Get bytes
            	chunk = m_socket->receiveMsg(&error);
                if (chunk.empty()) {
	                if (error == EAGAIN || error == EWOULDBLOCK)
	                	// No data yet, but connection alive
	                	return 1;
                	// TRUE EOF (peer closed)
                	return 0;
                }
				resetTimeOut();
                // 2. Delegation: Pass bytes to the Parser
                // The Client doesn't know about \r\n or Content-Length.
                // It just feeds the parser.
                m_request.appendData(chunk);
				// 3. Check State
                if (m_hasFullRequest == true
                	&& Init::logLevel == DEBUG) {
                	m_request.printRequest();
                }
			}
			catch (std::exception &e) {
            	// --- ADD LOGGING HERE ---
				Logger::MessagesFilter(ERR,
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
            m_socket->sendData(msg);
		}

		void reset(){
			m_request.reset();
			m_hasFullRequest = false;
		}

		bool isReceivingBody() const {
			return m_request.getExpectedSize() > m_request.getBody().size();
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