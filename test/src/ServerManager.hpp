/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 14:02:20 by yroard            #+#    #+#             */
/*   Updated: 2026/03/09 13:31:20 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Responsibility: Manages the select() loop, fd_sets, and the lifecycle of 
//sockets. It acts as the "Engine" of your server.
//Why: The while(true) loop, FD_SET manipulation, and accept() calls clutter
//main.

#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <vector>
#include "ConnectionManager.hpp"
#include "RequestHandler.hpp"
#include "SelectMultiplexer.hpp"
#include "serverConfig/ServerConfig.hpp"

namespace webserv {
	class ServerManager {
	private:
		IIOMultiplexer* m_multiplexer;
		ConnectionManager* m_connectionManager;
		
	public:
		ServerManager(IIOMultiplexer* multiplexer,
					const std::vector<serverConfig::ServerConfig*>& configs)
			: m_multiplexer(multiplexer), 
			m_connectionManager(new ConnectionManager(configs)){
		}
		
		~ServerManager() {
			delete m_multiplexer;
			delete m_connectionManager;
		}
		
		void setupServers() {
			m_connectionManager->setupServers(m_multiplexer);
		}
		
		void acceptNewConnection(int fd)const {
			m_connectionManager->addClient(fd, m_multiplexer);
		}
		
		bool isSendFailedFatally(Client* client){
			//if (FD_ISSET(fd, &m_writeSet)) {
				if (client->handleWrite() == -1) {
					return true;
				}
			//}
			return false;
		}
		//Listen for Writing: In handleClientActivity, if you have leftovers
		// or partial sends, ensure you call listenWriting.
		void handleClientActivity(int fd) {
			Logger::messagesFilter(INFO,
			"CASE B: It's a Client Socket!(Existing Connection)",
			"");
			Client* client = m_connectionManager->getClient(fd);
			int bytes = client->receiveData();
			Logger::messagesFilter(DEBUG,
			"Receiving data, Fd Client: ",
					ConvUtils::intToStr(fd));
			if (bytes < 0) {
				Logger::messagesFilter(ERR,
				"Error in receiving data",
						"client removed");
				m_connectionManager->removeClient(fd, m_multiplexer);
				return;
			}
			if (bytes == 0) {
				if (client->isReceivingBody()) {
					Logger::messagesFilter(DEBUG,
					"Client still to receive data: ",
							"no data received this time");
					return;
				}
				Logger::messagesFilter(DEBUG,
						"Data complete for this client: ",
						"client removed");
				m_connectionManager->removeClient(fd, m_multiplexer);
				return;
			}
			while (client->getRequest().isComplete()) {
				m_multiplexer->listenWriting(fd);
				client->setRequestAsFull();
				http::ParsingRequest& req = client->getRequest();
				const webserv::serverConfig::ServerConfig* config =
					client->getConfig();
				http::Response response =
					RequestHandler::handleRequest(req, *config);
				std::string finalMsg = response.httpString();
				client->sendResponse(finalMsg);
				std::string dataPreview(finalMsg);
				Logger::messagesFilter(DEBUG,
					"Response data preview: ",
					Logger::filterUnprintable(dataPreview));
				if (response.isConnectionToBeClosed()){
					Logger::messagesFilter(DEBUG,
						"Connection: close requested. Closing FD ",
						ConvUtils::intToStr(fd));
					// Instead of removing client immediately:
					if (response.isConnectionToBeClosed()) {
						client->setCloseConnection(true);
					}
					// m_connectionManager->removeClient(fd, m_multiplexer);
					return;
				}
				Logger::messagesFilter(INFO,
					"Keep-Alive: Waiting for next request on FD: ",
					ConvUtils::intToStr(fd));
				std::string leftovers = req.getRemainingData();
				m_connectionManager->resetClient(fd);
				if (!leftovers.empty()) {
					m_connectionManager->getClient(fd)
						->getRequest().appendData(leftovers);
				}
				else
					break;
			}
		}

		void run() {
			setupServers();
			while (!Init::stopRequested) {
				// 1. Prepare Write Set based on Clients who have data
        		// (You might need a helper in ConnectionManager to loop clients)
        		// For now, let's assume you do it via add/remove logic or just 
				// iterate: ideally, m_multiplexer should allow dynamic updates.
				int maxFd = m_connectionManager->getMaxFd();
				int activity = m_multiplexer->wait(maxFd);
				m_connectionManager->checkTimeouts(m_multiplexer);
				if (activity < 0) {
					Logger::messagesFilter(DEBUG,
						"Server Manager::run: activity < 0!", "");
					int errValue = errno;
					if (errValue == EINTR)
						continue;
					Logger::messagesFilter(ERR,
						"ServerManager::run: activity < 0, Select error:",
									strerror(errValue));
					continue;
				}
				std::vector<int> readyReadFds
					= m_multiplexer->getReadyReadFds();
				// PROCESS READS
				for (size_t i = 0; i < readyReadFds.size(); ++i){
					int fd = readyReadFds[i];
					if (m_connectionManager->isListener(fd)) {
						acceptNewConnection(fd);
					} else {
						handleClientActivity(fd);
					}
					m_connectionManager->checkTimeouts(m_multiplexer);
				}
				std::vector<int> writeFds = m_multiplexer->getReadyWriteFds();
				for (size_t i = 0; i < writeFds.size(); ++i) {
					int fd = writeFds[i];
					Client* client = m_connectionManager->getClient(fd);
					if (client) {
						if (isSendFailedFatally(client))
						{
							m_connectionManager->removeClient(fd, m_multiplexer);
						}
						else if (!client->hasPendingData()) {
							m_multiplexer->stopListeningWriting(fd);
							if (client->readyToClose()) {
								m_connectionManager->removeClient(
									fd, m_multiplexer);
							}
						}
					}
					m_connectionManager->checkTimeouts(m_multiplexer);
				}
			}
		}
	};
}
#endif