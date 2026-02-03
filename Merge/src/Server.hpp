/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:35:00 by yroard            #+#    #+#             */
/*   Updated: 2026/01/15 11:36:44 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>
#include <iostream>
#include "Endpoint.hpp"
#include "Socket.hpp"
#include "serverConfig/IServerConfig.hpp"

namespace webserv {
    class Server {
    public:
        static Socket listen(const std::string &ipAddr,
                             const unsigned short port) {
            using namespace webserv;
            // 1. Create the Endpoint object, which holds the address info
            const Endpoint endpoint = Endpoint::createIpv4Addr(ipAddr, port);
            // 2. Create the Socket object
            const Socket s(Socket::createTcpSocket());
            // 3. Listen on the socket using the endpoint information
            s.listenSocket(endpoint); // Pass endpoint object by reference
            std::cout << "Server listening on " << ipAddr << ":" << port << std::endl;
            return s;
        }
    };
}
#endif
