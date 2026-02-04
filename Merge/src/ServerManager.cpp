#include "ServerManager.hpp"

#include "RequestHandler.hpp"
#include "http/ParsingRequest.hpp"
#include "Server.hpp"
#include "http/Response.hpp"

namespace webserv {
	ServerManager::ServerManager(
			const std::vector<webserv::serverConfig::ServerConfig*>& tabServerConf) 
		: m_configs(tabServerConf), m_maxFd(0){
		FD_ZERO(&m_readSet);
		FD_ZERO(&m_writeSet); 
		FD_ZERO(&m_masterSet);
		
	}
	ServerManager::~ServerManager() {
		// 1. Clean up Clients
        for (std::map<int, Client*>::iterator it = m_clients.begin(); 
				it != m_clients.end(); ++it) {
            close(it->first); // Close FD
            delete it->second; // Delete Client Object
        }
        // 2. Clean up Listeners (Just close FDs, configs are deleted below)
        for (std::map<int, webserv::serverConfig::ServerConfig*>::iterator it = 
				m_listenerMap.begin(); it != m_listenerMap.end(); ++it) {
            close(it->first);
        }
		//3. Clean up Listener Sockets
		for (std::map<int, webserv::Socket*>::iterator it = m_listenerSockets.begin(); 
			it != m_listenerSockets.end(); ++it) {
			delete it->second; // This calls ~Socket(), which closes the FD
		}
    	m_listenerSockets.clear();
        // 4. Clean up Configs
        for (size_t i = 0; i < m_configs.size(); ++i) {
            delete m_configs[i];
        }
	}

	// Initializes listening sockets based on configs
	void ServerManager::setupServers(){
		for (size_t index = 0; index < m_configs.size(); ++index) {
            int port = m_configs[index]->getPort().getValue();
            std::string ip = m_configs[index]->getIPAddress().getValue();
            
            try {
                // 1. Get the Socket object (returned by value from Server::listen)
            	// Note: This relies on your Socket Copy Constructor "stealing" the FD
            	webserv::Socket tempSocket = webserv::Server::listen(ip, port);               
				// 2. Move it to the heap.
            	// We create a new pointer. The Copy Constructor will run, 
            	// taking the FD from 'tempSocket' and putting it into 'listenerSocket'.
            	webserv::Socket* listenerSocket = new webserv::Socket(tempSocket);
				// 3. Get the FD from the persistent pointer
            	int fd = listenerSocket->getSockFd();
				// 4. Add to Listener Map
                m_listenerMap[fd] = m_configs[index];
				// IMPORTANT: You need a place to store 'listenerSocket' so you can delete it 
				// in ~ServerManager(). If you don't store it, you leak memory.
				// If you are using the 'm_clients' map approach I gave previously, 
				// you might want to add a specific map for listeners:
				m_listenerSockets[fd] = listenerSocket; 
                // 5. Add to Select Set
                FD_SET(fd, &m_masterSet);
                if (fd > m_maxFd) 
					m_maxFd = fd;
                std::cout << "***Server listening on " << ip << ":" << port << " (FD: " << fd << ")" 
					<< std::endl;
            } 
            catch (std::exception &e) {
                std::cerr << "Failed to listen on " << ip << ":" << port << " -> " << e.what() 
					<< std::endl;
            }
        }
	}

	void ServerManager::run(){
		while (true) {
			std::cout << "***Server Manager started." << std::endl;
			m_readSet = m_masterSet;
			// Wait for activity
			int activity = select(m_maxFd + 1, &m_readSet, NULL, NULL, NULL);
			if (activity < 0) {
				std::cout << "***error: activity < 0!!!" << std::endl;
				// Check if it's just a signal interruption
				if (errno == EINTR)
					continue;
				std::cerr << "Select error: " << strerror(errno) << std::endl;
				return;
			}
			// Loop from 0 to max_fd to find which one is ready
			for (int fd = 0; fd <= m_maxFd; ++fd) {
				if (!FD_ISSET(fd, &m_readSet)) continue;	
				if (m_listenerMap.count(fd)) {
					// CASE A: It's a Listener (New Connection)
					addClient(fd);
				}
				else {
					handleClientActivity(fd);
				}
			}
		}
	}

	void ServerManager::addClient(int listenerFd) {
		// 1. Retrieve the Listener Socket object associated with this FD
		// We need the object to call the non-static method acceptSocket()
		std::cout << "***New connection !" << std::endl;
		if (m_listenerSockets.find(listenerFd) == m_listenerSockets.end()) {
			std::cerr << "Error: Listener FD " << listenerFd << " not found in socket map." << std::endl;
			return;
		}
		webserv::Socket* listener = m_listenerSockets[listenerFd];
		try {
			// 2. Accept the connection
			// This returns a temporary Socket object by value.
			// Internal logic of acceptSocket() already sets O_NONBLOCK.
			webserv::Socket tempSock = listener->acceptSocket();
			// 3. Move to Heap
			// We allocate a new Socket on the heap using the copy constructor.
			// Your Socket copy constructor "steals" the FD from tempSock.
			webserv::Socket* clientSock = new webserv::Socket(tempSock);
			// 4. Get the new Client FD
			int clientFd = clientSock->getSockFd();
			// 5. Find the config associated with this listener
			webserv::serverConfig::ServerConfig* config = m_listenerMap[listenerFd];
			// 6. Create the Client Object
			// Client takes ownership of 'clientSock' and will delete it in its destructor
			Client* newClient = new Client(clientSock, config);
			// 7. Update Maps and Select Set
			m_clients[clientFd] = newClient;
			FD_SET(clientFd, &m_masterSet);
			if (clientFd > m_maxFd) m_maxFd = clientFd;
			// Log is already handled inside acceptSocket, but you can add more here if needed
			std::cout << "***New Client added to map on FD " << clientFd << std::endl;
		}
		catch (std::exception &e) {
			// Handle errors (e.g., EMFILE if too many open files)
			std::cerr << "Failed to accept client: " << e.what() << std::endl;
		}
    }

    void ServerManager::removeClient(int fd) {
        if (m_clients.find(fd) == m_clients.end()) 
			return;
        std::cout << "***Removing Client FD " << fd << std::endl;
        // 1. Remove from set
        FD_CLR(fd, &m_masterSet);
        // 2. Delete Object (The Client destructor should close the FD)
        delete m_clients[fd];
        // 3. Remove from map
        m_clients.erase(fd);
    }

	void ServerManager::resetClient(int fd) {
		if (m_clients.find(fd) == m_clients.end()) 
			return;
		std::cout << "***Reset Client FD " << fd << std::endl;
		m_clients[fd]->reset();
	}

	void ServerManager::handleClientActivity(int fd){
		std::cout << "***CASE B: It's a Client Socket!(Existing Connection)" << std::endl;
		Client* client = m_clients[fd];
		// 1. Receive Data (Client accumulates it internally)
		int bytes = client->receiveData();
		//std::cout << "bytes (client->receiveData()): " << bytes << std::endl;
		if (bytes <= 0) {
			removeClient(fd);
			return;
		}
		// 2. Check if we have a full request
		if (client->hasRequest()) {
			webserv::http::ParsingRequest& req = client->getRequest();
			const webserv::serverConfig::ServerConfig* config = 
				client->getConfig();
			// 3. Routing Logic
			http::Response response =
				RequestHandler::handleRequest(req, *config);
			// 4. Send Response
			std::string finalMsg = response.httpString();
			client->sendResponse(finalMsg); // Using client's
											// send method
			std::cout << "***Response sent: [" << finalMsg << "]" << std::endl;
			// 5. Closing or not closing?
			if (response.isConnectionToBeClosed()){
				std::cout << "Connection: close requested. Closing FD " << fd << std::endl;
				removeClient(fd);
			}
			else {
				// KEEP ALIVE:
				std::cout << "Keep-Alive: Waiting for next request on FD " << fd << std::endl;
				// IMPORTANT: Reset the client so it can read the next request
				resetClient(fd);
				// The FD remains in m_readSet via m_masterSet, so select() will pick it up
				// when the client sends the next request.
			}
		}
	}
}