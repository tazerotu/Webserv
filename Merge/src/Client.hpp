/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 14:09:53 by yroard            #+#    #+#             */
/*   Updated: 2026/01/27 13:33:41 by yroard           ###   ########.fr       */
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

#include <cstdlib>
#include "serverConfig/ServerConfig.hpp"
#include "http/ParsingRequest.hpp"

namespace webserv {
	class Client {
	private:
		Socket* 						m_socket;	// The mechanism
		const serverConfig::ServerConfig*     m_config;   // The rules
		// Data management
        webserv::http::ParsingRequest 	m_request; 
		std::string 					m_rawReceivedData;
		bool 							m_hasFullRequest;

	public:
	// Client takes ownership of the Socket pointer
		Client(Socket* socket, const serverConfig::ServerConfig* config)
			: m_socket(socket), m_config(config), m_hasFullRequest(false) {}
			
		~Client(){
			delete m_socket; // Client destroys the connection when it dies
		}

		// Returns the FD
		int getFd() const{
			return m_socket->getSockFd();
		}

		const serverConfig::ServerConfig* getConfig() const {
			return m_config; 
		}

		// Reads data from socket into request parser
		// Returns bytes read (0 = disconnected, -1 = error)
		int receiveData(){
			// 1. Use Socket to get bytes
			std::string chunk = "";
            try {
            	int returnErrorValue = 0;
				chunk = m_socket->receiveMsg(&returnErrorValue);
                if (chunk.empty()) {
	                if (returnErrorValue == EAGAIN 
							|| returnErrorValue == EWOULDBLOCK)
	                	return 1;
                	return 0; // Connection closed
                }
                // 2. Accumulate bytes
                m_rawReceivedData += chunk;
            	// 3. Calculate Total Expected Size
            	// Total = Headers Size + 4 bytes (\r\n\r\n) + Body Size
				const size_t headerEndPos = m_rawReceivedData.find("\r\n\r\n");
				std::cout << "headerEndPos= " << headerEndPos << std::endl;
            	if (headerEndPos != std::string::npos) {
            		std::string headers = m_rawReceivedData.substr(0,
            			headerEndPos);
					std::cout << "Headers: [[[" << headers << "]]]" << std::endl;
            		//check for body size
					long bodyLength = 0;
            		size_t startPosCL =	headers.find("Content-Length:");
					std::cout << "startPosCL= " << startPosCL << std::endl;
					if (startPosCL != std::string::npos) {
						// Case Content-Length exists
						size_t endPosBodySize = headers.find("\r\n", startPosCL);
						std::cout << "endPosBodySize= " << endPosBodySize << std::endl;
						if (endPosBodySize == std::string::npos)
							endPosBodySize = headers.length();					
						std::cout << "endPosBodySize= " << endPosBodySize << std::endl;
						std::string bodySize = m_rawReceivedData.substr(
							startPosCL + 15, endPosBodySize - startPosCL - 15);
						std::cout << "bodySize= " << bodySize << std::endl;
						bodyLength = strtol(bodySize.c_str(), NULL,10);
					}
					size_t totalSize = headerEndPos + 4 + bodyLength;
					if (m_rawReceivedData.size() >= headerEndPos + 4 + bodyLength){
						std::string reqData = m_rawReceivedData.substr(0, totalSize);
						Buffer buf(reqData);
						m_request = http::ParsingRequest::parseRequest(buf);
						m_hasFullRequest = true;
					}           		
            	}
            }
			catch (std::exception &e) {
            	// --- ADD LOGGING HERE ---
				std::cerr << "Error during request parsing: " << e.what() << std::endl;
				return -1;
            }
			return 1;
		}

		bool hasRequest() const { return m_hasFullRequest; }

        http::ParsingRequest& getRequest() { return m_request; }

        void sendResponse(const std::string& msg) {
            m_socket->sendData(msg);
		}

		void reset(){
			m_rawReceivedData.erase();
			m_request.reset();
			m_hasFullRequest = false;
		}
	};
}

#endif