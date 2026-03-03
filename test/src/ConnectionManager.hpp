//
// Created by yannou on 10/02/2026.
//

#ifndef WEBSERV_CONNECTIONMANAGER_HPP
#define WEBSERV_CONNECTIONMANAGER_HPP
#include <map>
#include <vector>
#include "Client.hpp"
#include "SelectMultiplexer.hpp"
#include "Server.hpp"
#include "Socket.hpp"

namespace webserv {
	class ConnectionManager {
	private:
		const std::vector<webserv::serverConfig::ServerConfig*> m_configs;
		int    m_maxFd;
		std::map<int, Client*> m_clients;
		std::map<int, Socket*> m_listenerSockets;
		std::map<int, serverConfig::ServerConfig*> m_listenConfig;

	public:
		explicit ConnectionManager(
			const std::vector<serverConfig::ServerConfig*>& configs)
				: m_configs(configs), m_maxFd(0) {

		}

		~ConnectionManager() {
			closeAll();
		}
		
		void setupServers(IIOMultiplexer* multiplexer){
			for (size_t index = 0; index < m_configs.size(); ++index) {
				int port = m_configs[index]->getPort().getValue();
				std::string ip = m_configs[index]->getIPAddress().getValue();

				try {
					Socket tempSocket = Server::listen(ip, port);
					Socket* listenerSocket = new Socket(tempSocket);
					int fd = listenerSocket->getSockFd();
					m_listenConfig[fd] = m_configs[index];
					m_listenerSockets[fd] = listenerSocket;
					if (fd > m_maxFd)
						m_maxFd = fd;
					multiplexer->addFd(fd);
					std::stringstream resultInfo;
					resultInfo << ip << ":" << port << " (FD: " << fd << ")";
					Logger::messagesFilter(INFO,
						"Server listening on ",
						resultInfo.str());
				}
				catch (std::exception &e) {
					Logger::messagesFilter(ERR,
						"Server setup failed: ",
									e.what());
				}
			}
		}
	    void addClient(int listenerFd, IIOMultiplexer* multiplexer) {
			Logger::messagesFilter(INFO,
			"New connection", "");
			if (m_listenerSockets.find(listenerFd) == m_listenerSockets.end()) {
				return;
			}
			try {
				Socket* listener = m_listenerSockets[listenerFd];
				Socket tempSock = listener->acceptSocket();
				Socket* clientSock = new Socket(tempSock);
				int clientFd = clientSock->getSockFd();
				serverConfig::ServerConfig* config =
					m_listenConfig[listenerFd];
				Client* newClient = new Client(clientSock, config);
				m_clients[clientFd] = newClient;
				multiplexer->addFd(clientFd);
				if (clientFd > m_maxFd)
					m_maxFd = clientFd;
				Logger::messagesFilter(DEBUG,
					"New Client added to map on FD ",
					ConvUtils::intToStr(clientFd));
				if (newClient->hasPendingData()) {
        			multiplexer->listenWriting(clientFd);
    			}
			}
			catch (std::exception &e) {
				Logger::messagesFilter(ERR,
					"Failed to accept client: ",
					e.what());
			}

	    }
	    void removeClient(int fd, IIOMultiplexer* multiplexer) {
	    	if (m_clients.find(fd) == m_clients.end())
	    		return;
	    	Logger::messagesFilter(INFO,
				"Removing Client FD ","");
			multiplexer->removeFd(fd);
	    	delete m_clients[fd];
	    	m_clients.erase(fd);
	    }
		void resetClient(int fd) {
	    	if (m_clients.find(fd) == m_clients.end())
	    		return;
	    	Logger::messagesFilter(DEBUG,
				 "Reset Client FD: ",
				 ConvUtils::intToStr(fd));
	    	m_clients[fd]->reset();
	    }
	    Client* getClient(int fd) {
			if (m_clients.find(fd) != m_clients.end())
				return m_clients[fd];
			return NULL;
		}
		int getMaxFd() {
			return m_maxFd;
		}
		bool isListener(int fd) {
			if (m_listenConfig.find(fd) != m_listenConfig.end())
				return true;
			return false;
		}
	    void checkTimeouts(IIOMultiplexer* multiplexer) {
			std::map<int, Client*>::iterator it = m_clients.begin();
			while (it != m_clients.end()){
	    		if (it->second->hasTimedOut(Init::timeOutLimit)) {
	    			int fd = it->first;
	    			Logger::messagesFilter(ERR,
					"Client timed out, closing FD: ",
					ConvUtils::intToStr(fd));
	    			multiplexer->removeFd(fd);
	    			delete it->second;
	    			std::map<int, Client*>::iterator toErase = it;
                    ++it; 
                    m_clients.erase(toErase);
                } 
                else {
                    ++it;
				}
	    	}
	    }
	    void closeAll(){
	    	for (std::map<int, Client*>::iterator it = m_clients.begin();
					it != m_clients.end(); ++it) {
	    		close(it->first);
	    		delete it->second;
					}
	    	for (std::map<int, webserv::serverConfig::ServerConfig*>::iterator it =
					m_listenConfig.begin(); it != m_listenConfig.end(); ++it) {
	    		close(it->first);
					}
	    	for (std::map<int, Socket*>::iterator it = m_listenerSockets.begin();
				it != m_listenerSockets.end(); ++it) {
	    		delete it->second;
				}
	    	m_listenerSockets.clear();
	    	for (size_t i = 0; i < m_configs.size(); ++i) {
	    		delete m_configs[i];
	    	}
	    }
	};
}

#endif //WEBSERV_CONNECTIONMANAGER_HPP