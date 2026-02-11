#include "ServerManager.hpp"

#include "RequestHandler.hpp"
#include "http/ParsingRequest.hpp"
#include "Server.hpp"
#include "http/Response.hpp"

//namespace webserv {
	// ServerManager::ServerManager(
	// 		const std::vector<webserv::serverConfig::ServerConfig*>& tabServerConf)
	// 	: m_configs(tabServerConf), m_maxFd(0){
	// 	FD_ZERO(&m_readSet);
	// 	FD_ZERO(&m_writeSet);
	// 	FD_ZERO(&m_masterSet);
	//
	// }
	// ServerManager::~ServerManager() {
	// 	// 1. Clean up Clients
 //        for (std::map<int, Client*>::iterator it = m_clients.begin();
	// 			it != m_clients.end(); ++it) {
 //            close(it->first); // Close FD
 //            delete it->second; // Delete Client Object
 //        }
 //        // 2. Clean up Listeners (Just close FDs, configs are deleted below)
 //        for (std::map<int, webserv::serverConfig::ServerConfig*>::iterator it =
	// 			m_listenerMap.begin(); it != m_listenerMap.end(); ++it) {
 //            close(it->first);
 //        }
	// 	//3. Clean up Listener Sockets
	// 	for (std::map<int, Socket*>::iterator it = m_listenerSockets.begin();
	// 		it != m_listenerSockets.end(); ++it) {
	// 		delete it->second; // This calls ~Socket(), which closes the FD
	// 	}
 //    	m_listenerSockets.clear();
 //        // 4. Clean up Configs
 //        for (size_t i = 0; i < m_configs.size(); ++i) {
 //            delete m_configs[i];
 //        }
	// }

	// Initializes listening sockets based on configs
	// void ServerManager::setupServers(){
	// 	for (size_t index = 0; index < m_configs.size(); ++index) {
 //            int port = m_configs[index]->getPort().getValue();
 //            std::string ip = m_configs[index]->getIPAddress().getValue();
 //
 //            try {
 //                // 1. Get the Socket object (returned by value from Server::listen)
 //            	Socket tempSocket = Server::listen(ip, port);
	// 			// 2. Move it to the heap.
 //            	Socket* listenerSocket = new Socket(tempSocket);
	// 			// 3. Get the FD from the persistent pointer
 //            	int fd = listenerSocket->getSockFd();
	// 			// 4. Add to Listener Map
 //                m_listenerMap[fd] = m_configs[index];
	// 			m_listenerSockets[fd] = listenerSocket;
 //                // 5. Add to Select Set
 //                FD_SET(fd, &m_masterSet);
 //                if (fd > m_maxFd)
	// 				m_maxFd = fd;
 //            	std::stringstream resultInfo;
 //            	resultInfo << ip << ":" << port << " (FD: " << fd << ")";
 //                Logger::MessagesFilter(INFO,
 //                	"Server listening on ",
	// 				resultInfo.str());
 //            }
 //            catch (std::exception &e) {
 //            	std::stringstream resultErr;
 //            	resultErr << ip << ":" << port;
 //            	Logger::MessagesFilter(ERR, "Server listening on ",
	// 							resultErr.str());
 //            }
 //        }
	// }

// void ServerManager::checkForTimeOut() {
// 		int fd = 0;
// 		for (fd = 0; fd < m_maxFd; ++fd) {
// 			if (m_clients[fd] == NULL)
// 				continue;
// 			Client* client = m_clients[fd];
// 			if (client->isSockTimeOut(Init::TimeOutLimit)) {
// 				Logger::MessagesFilter(ERR,
// 				"Client timed out, closing connection.",
// 				"");
// 				// The Socket destructor will close the FD automatically
// 				removeClient(fd);
// 			}
// 		}
// 	}

//	vo?

	// void ServerManager::addClient(int listenerFd) {
	// 	// 1. Retrieve the Listener Socket object associated with this FD
	// 	// We need the object to call the non-static method acceptSocket()
	// 	Logger::MessagesFilter(INFO,
	// 		"New connection", "");
	// 	if (m_listenerSockets.find(listenerFd) == m_listenerSockets.end()) {
	// 		std::stringstream lFd;
	// 		lFd << listenerFd << "not found in socket map";
	// 		Logger::MessagesFilter(ERR,
	// 			"ServerManager::addClient: Listener FD ",
	// 			lFd.str());
	// 		return;
	// 	}
	// 	webserv::Socket* listener = m_listenerSockets[listenerFd];
	// 	try {
	// 		// 2. Accept the connection
	// 		// This returns a temporary Socket object by value.
	// 		// Internal logic of acceptSocket() already sets O_NONBLOCK.
	// 		Socket tempSock = listener->acceptSocket();
	// 		// 3. Move to Heap
	// 		// We allocate a new Socket on the heap using the copy constructor.
	// 		// Your Socket copy constructor "steals" the FD from tempSock.
	// 		webserv::Socket* clientSock = new webserv::Socket(tempSock);
	// 		// 4. Get the new Client FD
	// 		int clientFd = clientSock->getSockFd();
	// 		// 5. Find the config associated with this listener
	// 		webserv::serverConfig::ServerConfig* config = m_listenerMap[listenerFd];
	// 		// 6. Create the Client Object
	// 		// Client takes ownership of 'clientSock' and will delete it in its destructor
	// 		Client* newClient = new Client(clientSock, config);
	// 		// 7. Update Maps and Select Set
	// 		m_clients[clientFd] = newClient;
	// 		FD_SET(clientFd, &m_masterSet);
	// 		if (clientFd > m_maxFd)
	// 			m_maxFd = clientFd;
	// 		// Log is already handled inside acceptSocket,
	// 		// but you can add more here if needed
	// 		Logger::MessagesFilter(DEBUG,
	// 			"New Client added to map on FD ",
	// 			ConvUtils::intToStr(clientFd));
	// 	}
	// 	catch (std::exception &e) {
	// 		// Handle errors (e.g., EMFILE if too many open files)
	// 		Logger::MessagesFilter(ERR,
	// 			"Failed to accept client: ",
	// 			e.what());
	// 	}
 //    }

   //  void ServerManager::removeClient(int fd) {
   //      if (m_clients.find(fd) == m_clients.end())
			// return;
   //      Logger::MessagesFilter(INFO,
   //      	"Removing Client FD ","");
   //      // 1. Remove from set
   //      FD_CLR(fd, &m_masterSet);
   //      // 2. Delete Object (The Client destructor should close the FD)
   //      delete m_clients[fd];
   //      // 3. Remove from map
   //      m_clients.erase(fd);
   //  }

	// void ServerManager::resetClient(int fd) {
	// 	if (m_clients.find(fd) == m_clients.end())
	// 		return;
	// 	Logger::MessagesFilter(DEBUG,
	// 		 "Reset Client FD: ",
	// 		 ConvUtils::intToStr(fd));
	// 	m_clients[fd]->reset();
	// }

	// The FD remains in m_readSet via m_masterSet, so select() will pick it up
	// when the client sends the next request.
	// void ServerManager::handleClientActivity(int fd){
	// 	Logger::MessagesFilter(INFO,
	// 		"CASE B: It's a Client Socket!(Existing Connection)",
	// 		"");
	// 	Client* client = m_clients[fd];
	// 	// 1. Receive Data (Client accumulates it internally)
	// 	int bytes = client->receiveData();
	// 	Logger::MessagesFilter(DEBUG,
	// 	"Receiving data, Fd Client: ",
	// 			ConvUtils::intToStr(fd));
	// 	if (bytes < 0) {
	// 		Logger::MessagesFilter(ERR,
	// 		"Error in receiving data",
	// 				"client removed");
	// 		removeClient(fd);
	// 		return;
	// 	}
	// 	if (bytes == 0) {
	// 		if (client->isReceivingBody()) {
	// 			Logger::MessagesFilter(DEBUG,
	// 			"Client still to receive data: ",
	// 					"no data received this time");
	// 			return;
	// 		}
	// 		Logger::MessagesFilter(DEBUG,
	// 				"Data complete for this client: ",
	// 				"client removed");
	// 		removeClient(fd);
	// 		return;
	// 	}
	// 	// 2. Loop to handle potentially multiple requests (Pipelining)
	// 	// We loop as long as the request is complete
	// 	while (client->getRequest().isComplete()) {
	// 		client->setRequestAsFull();
	// 		http::ParsingRequest& req = client->getRequest();
	// 		const webserv::serverConfig::ServerConfig* config =
	// 			client->getConfig();
	// 		http::Response response =
	// 			RequestHandler::handleRequest(req, *config);
	// 		std::string finalMsg = response.httpString();
	// 		client->sendResponse(finalMsg); // Using client's send method
	// 		std::string dataPreview(finalMsg);
	// 		Logger::MessagesFilter(DEBUG,
	// 			"Response data preview: ",
	// 			Logger::filterUnprintable(dataPreview));
	// 		if (response.isConnectionToBeClosed()){
	// 			Logger::MessagesFilter(DEBUG,
	// 				"Connection: close requested. Closing FD ",
	// 				ConvUtils::intToStr(fd));
	// 			removeClient(fd);
	// 			return;
	// 		}
	// 		Logger::MessagesFilter(INFO,
	// 			"Keep-Alive: Waiting for next request on FD: ",
	// 			ConvUtils::intToStr(fd));
	// 		// Check for leftovers
	// 		std::string leftovers = req.getRemainingData();
	// 		resetClient(fd);
	// 		if (!leftovers.empty()) {
	// 			m_clients[fd]->getRequest().appendData(leftovers);
	// 		}
	// 		else
	// 			break;
	// 	}
//	}
//}