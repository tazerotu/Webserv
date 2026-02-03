/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 09:31:11 by ttas              #+#    #+#             */
/*   Updated: 2025/11/22 08:35:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <vector>
#include "serverConfig/ServerConfig.hpp"
#include "Request.hpp"
#include "Server.hpp"
#include "Socket.hpp"
#include "test/test.hpp"
#include "serverConfig/IServerConfig.hpp"
// #include <iostream>

int main(int argc, char **argv) {
    webserv::test::testIPAddress();
    webserv::test::testPort();
    webserv::test::testMaxBodySize();
    typedef webserv::serverConfig::ServerConfig ServConfig;
    std::vector <ServConfig> serverConf;
    ServConfig serverConf1(webserv::serverConfig::ServiceConfigWebsiteName::create("Hello"),
        webserv::serverConfig::ServiceConfigIPAddress::create("127.0.0.1"),
        webserv::serverConfig::ServiceConfigPort::create(8080),
        webserv::serverConfig::ServiceConfigMaxBodySize::create(100000));
    serverConf.push_back(serverConf1);
    ServConfig serverConf2(webserv::serverConfig::ServiceConfigWebsiteName::create("Hello"),
        webserv::serverConfig::ServiceConfigIPAddress::create("192.168.0.1"),
        webserv::serverConfig::ServiceConfigPort::create(9000),
        webserv::serverConfig::ServiceConfigMaxBodySize::create(100));
    serverConf.push_back(serverConf2);
    // virtual const Routes::ServiceConfigRootPath &rootPath() = 0;
    // virtual const Routes::ServiceConfigErrorPages &errorPages() = 0;
    // virtual Routes::ServiceConfigAutoIndex autoIndex() = 0;
    std::map<int, ServConfig*> socketConfigMap;
    fd_set master_set;      // The permanent list
    fd_set read_set;        // The temp list for select()
    FD_ZERO(&master_set);   // Clear the set
    int max_fd = 0;
    std::vector<webserv::Socket> tcpSocketTab;
    for (size_t i = 0; i < serverConf.size(); ++i) {
        webserv::Socket tcpSocket =
                        webserv::Server::listen(serverConf[i].getIPAddress().getValue(),
                            serverConf[i].getPort().getValue());
        tcpSocketTab.push_back(tcpSocket);
        if (tcpSocket.getSockFd() > max_fd)
            max_fd = tcpSocket.getSockFd();
        FD_SET(tcpSocket.getSockFd(), &master_set);
        socketConfigMap[tcpSocket.getSockFd()] = &serverConf[i];
    }
    // 2. MAIN LOOP
    while (true) {
        // IMPORTANT: Re-initialize the set every loop!
        // We copy master_set into read_set because select() destroys read_set
        read_set = master_set;

        // Wait for activity
        // param 1: highest FD + 1
        // param 2: read set (waiting for data in)
        // param 3: write set (waiting to write - optional for now)
        // param 4: exception set
        // param 5: timeout (NULL = wait forever)
        int activity = select(max_fd + 1, &read_set, NULL, NULL, NULL);
        if (activity < 0) {
            std::cout << "error: activity < 0!!!" << std::endl;
            break; // Error
        }
        // 3. CHECK FDs
        // Loop from 0 to max_fd to find which one is ready
        for (int i = 0; i <= max_fd; ++i) {
            // Is 'i' inside the set of ready FDs?
            if (FD_ISSET(i, &read_set)) {
                // CASE A: It's a Listening Socket (New Connection)
                if (socketConfigMap.count(i)) {
                    std::vector<webserv::Socket>::iterator it;
                    std::vector<webserv::Socket>::iterator itStart =
                            tcpSocketTab.begin();
                    std::vector<webserv::Socket>::iterator itEnd =
                            tcpSocketTab.end();
                    unsigned index = 0;
                    for (it = itStart ; it != itEnd ; ++it, ++index) {
                        if (tcpSocketTab[index].getSockFd() == i) {
                            webserv::Socket clientConnect =
                            tcpSocketTab[index].acceptSocket();
                            // Add new client to the MASTER set
                            FD_SET(clientConnect.getSockFd(), &master_set);
                            // Update max_fd if needed
                            if (clientConnect.getSockFd() > max_fd)
                                max_fd = clientConnect.getSockFd();
                        }
                    }
                }
                // CASE B: It's a Client Socket (Existing Connection)
                else {
                    tcpSocketTab[index].getSockFd()
                    // char buffer[1024];
                    // int bytes = recv(i, buffer, 1024, 0);
                    //
                    // if (bytes <= 0) {
                    //     // Disconnect
                    //     close(i);
                    //     FD_CLR(i, &master_set); // Remove from master set
                    //     // Optional: Re-calculate max_fd here to be precise,
                    //     // but not strictly required (select handles gaps fine)
                    // } else {
                    //     // Handle Request
                    // }
                }
            }
        }
    }
    return 0;
}
    // try {
    //     webserv::Socket clientSocket =
    //             webserv::Server::listen(serverConf[0].getIPAddress().getValue(), serverConf[0].getPort().getValue());
    //     clientSocket.receiveMsg();
    //     clientSocket.parseReceivedMsg(serverConf[0].getMaxBodySize().getMaxBodySize());
    //
    //     // webserv::Request::Receive(fd);
    //     // webserv::Response::Send(fd);
    // } catch
    // (std::exception &e) {
    //     std::cerr << "Error: " << e.what() << std::endl;
    //     return 1;
    // }
//     return 0;
// }

/*
    if(argc != 2)
    {
        std::cout << "Wrong amount of arguments!" << std::endl << "correct syntax : ./webserv [path/to/file.conf]" << std::endl;
        return(0);
    }
    class ServerConfig serverConfig(argv[1]);

    std::cout << "ServerConfig values:" << std::endl;
    std::cout << "Name: " << serverConfig.getName() << std::endl;
    std::cout << "Host: " << serverConfig.getHost() << std::endl;
    std::cout << "Port: " << serverConfig.getPort() << std::endl;
    std::cout << "Root: " << serverConfig.getRoot() << std::endl;
    std::cout << "Index: " << serverConfig.getIndex() << std::endl;
    std::cout << "Autoindex: " << serverConfig.getAutoindex() << std::endl;

    // ---- Error pages ----
    const std::map<std::string, std::string> &errors = serverConfig.getErrorPages();
    std::cout << "Error pages: ";
    if (errors.empty())
        std::cout << "(none)";
    else
    {
        bool first = true;
        for (std::map<std::string, std::string>::const_iterator it = errors.begin();
            it != errors.end(); ++it)
        {
            if (!first)
                std::cout << ", ";
            std::cout << it->first << ": " << it->second;
            first = false;
        }
    }
    std::cout << std::endl;
    // ---- Allowed methods ----
    const std::vector<std::string> &methods = serverConfig.getAllowedMethods();
    std::cout << "Allowed methods: ";
    if (methods.empty())
        std::cout << "(none)";
    else
    {
        for (std::vector<std::string>::const_iterator it = methods.begin();
            it != methods.end(); ++it)
        {
            std::cout << *it;
            if (it + 1 != methods.end())
                std::cout << ", ";
        }
    }
    std::cout << std::endl;

    std::cout << "Max client body size: " << serverConfig.getMaxClientBodySize() << std::endl;
    std::cout << "CGI path: " << serverConfig.getCgiPath() << std::endl;
    std::cout << "CGI extension: " << serverConfig.getCgiExt() << std::endl;

    */