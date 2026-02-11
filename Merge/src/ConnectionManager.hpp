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
	    // to store the actual Listener Socket objects (FD -> Socket*)
		std::map<int, Socket*> m_listenerSockets;
	    // logic to map Listener FD to Config ...
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
					// 1. Get the Socket object (returned by value from Server::listen)
					Socket tempSocket = Server::listen(ip, port);
					// 2. Move it to the heap.
					Socket* listenerSocket = new Socket(tempSocket);
					// 3. Get the FD from the persistent pointer
					int fd = listenerSocket->getSockFd();
					// 4. Add to Listener Map
					m_listenConfig[fd] = m_configs[index];
					m_listenerSockets[fd] = listenerSocket;
					if (fd > m_maxFd)
						m_maxFd = fd;
					multiplexer->addFd(fd);
					std::stringstream resultInfo;
					resultInfo << ip << ":" << port << " (FD: " << fd << ")";
					Logger::MessagesFilter(INFO,
						"Server listening on ",
						resultInfo.str());
				}
				catch (std::exception &e) {
					Logger::MessagesFilter(ERR,
						"Server setup failed: ",
									e.what());
				}
			}
		}
	    void addClient(int listenerFd, IIOMultiplexer* multiplexer) {
			// 1. Retrieve the Listener Socket object associated with this FD
			// We need the object to call the non-static method acceptSocket()
			Logger::MessagesFilter(INFO,
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
				Logger::MessagesFilter(DEBUG,
					"New Client added to map on FD ",
					ConvUtils::intToStr(clientFd));
			}
			catch (std::exception &e) {
				// Handle errors (e.g., EMFILE if too many open files)
				Logger::MessagesFilter(ERR,
					"Failed to accept client: ",
					e.what());
			}

	    }
	    void removeClient(int fd, IIOMultiplexer* multiplexer) {
	    	if (m_clients.find(fd) == m_clients.end())
	    		return;
	    	Logger::MessagesFilter(INFO,
				"Removing Client FD ","");
			multiplexer->removeFd(fd);
	    	delete m_clients[fd];
	    	m_clients.erase(fd);
			if (fd == m_maxFd)
				--m_maxFd;
	    }
		void resetClient(int fd) {
	    	if (m_clients.find(fd) == m_clients.end())
	    		return;
	    	Logger::MessagesFilter(DEBUG,
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
	    		if (it->second->hasTimedOut(Init::TimeOutLimit)) {
	    			int fd = it->first;
	    			Logger::MessagesFilter(ERR,
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
	    	// 1. Clean up Clients
	    	for (std::map<int, Client*>::iterator it = m_clients.begin();
					it != m_clients.end(); ++it) {
	    		close(it->first); // Close FD
	    		delete it->second; // Delete Client Object
					}
	    	// 2. Clean up Listeners (Just close FDs, configs are deleted below)
	    	for (std::map<int, webserv::serverConfig::ServerConfig*>::iterator it =
					m_listenConfig.begin(); it != m_listenConfig.end(); ++it) {
	    		close(it->first);
					}
	    	//3. Clean up Listener Sockets
	    	for (std::map<int, Socket*>::iterator it = m_listenerSockets.begin();
				it != m_listenerSockets.end(); ++it) {
	    		delete it->second; // This calls ~Socket(), which closes the FD
				}
	    	m_listenerSockets.clear();
	    	// 4. Clean up Configs
	    	for (size_t i = 0; i < m_configs.size(); ++i) {
	    		delete m_configs[i];
	    	}
	    } // Destructor logic goes here

	    // Accessors for ServerManager to loop over
	    //std::vector<int> getAllFds() const {	    }
	};
}

#endif //WEBSERV_CONNECTIONMANAGER_HPP