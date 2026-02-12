/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 14:02:20 by yroard            #+#    #+#             */
/*   Updated: 2026/02/12 09:24:50 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Responsibility: Manages the select() loop, fd_sets, and the lifecycle of sockets.
// It acts as the "Engine" of your server.
//Why: The while(true) loop, FD_SET manipulation, and accept() calls clutter main.

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
		void handleClientActivity(int fd) {
			Logger::MessagesFilter(INFO,
			"CASE B: It's a Client Socket!(Existing Connection)",
			"");
			Client* client = m_connectionManager->getClient(fd);
			// 1. Receive Data (Client accumulates it internally)
			int bytes = client->receiveData();
			Logger::MessagesFilter(DEBUG,
			"Receiving data, Fd Client: ",
					ConvUtils::intToStr(fd));
			if (bytes < 0) {
				Logger::MessagesFilter(ERR,
				"Error in receiving data",
						"client removed");
				m_connectionManager->removeClient(fd, m_multiplexer);
				return;
			}
			if (bytes == 0) {
				if (client->isReceivingBody()) {
					Logger::MessagesFilter(DEBUG,
					"Client still to receive data: ",
							"no data received this time");
					return;
				}
				Logger::MessagesFilter(DEBUG,
						"Data complete for this client: ",
						"client removed");
				m_connectionManager->removeClient(fd, m_multiplexer);
				return;
			}
			// 2. Loop to handle potentially multiple requests (Pipelining)
			// We loop as long as the request is complete
			while (client->getRequest().isComplete()) {
				client->setRequestAsFull();
				http::ParsingRequest& req = client->getRequest();
				const webserv::serverConfig::ServerConfig* config =
					client->getConfig();
				http::Response response =
					RequestHandler::handleRequest(req, *config);
				std::string finalMsg = response.httpString();
				client->sendResponse(finalMsg); // Using client's send method
				std::string dataPreview(finalMsg);
				Logger::MessagesFilter(DEBUG,
					"Response data preview: ",
					Logger::filterUnprintable(dataPreview));
				if (response.isConnectionToBeClosed()){
					Logger::MessagesFilter(DEBUG,
						"Connection: close requested. Closing FD ",
						ConvUtils::intToStr(fd));
					m_connectionManager->removeClient(fd, m_multiplexer);
					return;
				}
				Logger::MessagesFilter(INFO,
					"Keep-Alive: Waiting for next request on FD: ",
					ConvUtils::intToStr(fd));
				// Check for leftovers
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
				// 1. Wait (Blocking)
				int maxFd = m_connectionManager->getMaxFd();
				int activity = m_multiplexer->wait(maxFd);
				if (activity < 0) {
					Logger::MessagesFilter(DEBUG,
					"Server Manager::run: activity < 0!", "");
					// Check if it's just a signal interruption
					int errValue = errno;
					if (errValue == EINTR)
						continue;
					Logger::MessagesFilter(ERR,
						"ServerManager::run: activity < 0, Select error:",
									strerror(errValue));
					continue;
				}
				// 2. Get Ready FDs (Clean list, no looping 0..maxFd)
				std::vector<int> readyFds
					= m_multiplexer->getReadyFds();
				for (size_t i = 0; i < readyFds.size(); ++i){
					int fd = readyFds[i];
					if (m_connectionManager->isListener(fd)) {
						// Delegate acceptance logic
						acceptNewConnection(fd);
					} else {
						// Delegate business logic
						handleClientActivity(fd);
					}
					m_connectionManager->checkTimeouts(m_multiplexer);
				}
				// 3. Delegate cleanup
				m_connectionManager->checkTimeouts(m_multiplexer);

				// Verify how long since last activity, if > 30s -> cut connection

				/*
					timeout test (diminuer le temps): 
					- printf "" | nc localhost 8080 
					- printf "GET / HTTP/1.1\r\nHost: localhost:8080\r\n\r\n" | nc localhost 8080 
					- boucle infini en CGI /// division par 0

					Code dans les fichiers .cpp au lieu des .hpp

					Test de montee de charge avec `siege` avec le maximum de client (255) (succes de 99.7% minimum)
				*/
			}
		}
	};
}
#endif