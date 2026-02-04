/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/13 14:02:20 by yroard            #+#    #+#             */
/*   Updated: 2026/02/04 09:31:28 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//Responsibility: Manages the select() loop, fd_sets, and the lifecycle of sockets.
// It acts as the "Engine" of your server.
//Why: The while(true) loop, FD_SET manipulation, and accept() calls clutter main.

#ifndef SERVERMANAGER_HPP
#define SERVERMANAGER_HPP

#include <map>
#include <vector>
#include <sys/select.h>
#include "Socket.hpp"
#include "Client.hpp" // See next point


namespace webserv {
	class ServerManager {
	private:
		const std::vector<webserv::serverConfig::ServerConfig*> m_configs;
		// Map 1: Listening Sockets (FD -> Config)
        // Used to know which config applies when accepting a new connection
        std::map<int, webserv::serverConfig::ServerConfig*> m_listenerMap;

        // Map 2: Active Clients (FD -> Client Object)
        // Used to store state (buffer, request) for each connection
        std::map<int, Client*> m_clients;

		// Map 3: to store the actual Listener Socket objects (FD -> Socket*)
    	std::map<int, webserv::Socket*> m_listenerSockets; 

		fd_set m_readSet;
		fd_set m_writeSet;
		fd_set m_masterSet;
		int    m_maxFd;

	public:
		explicit ServerManager(const std::vector<webserv::serverConfig::ServerConfig*>& tabServerConf);
		~ServerManager();
		// Initializes listening sockets based on configs
		void setupServers();

		// The infinite loop (select)
		void run();

	private:
        // Helper to add/remove/reset client clearly
        void removeClient(int fd);
        void addClient(int listenerFd);
		void resetClient(int fd);
		void handleClientActivity(int fd);
		
	};
}
#endif