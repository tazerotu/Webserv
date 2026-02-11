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
//#include <iostream>
//#include <stdlib.h>

#include "Endpoint.hpp"
#include "Socket.hpp"
//#include "serverConfig/IServerConfig.hpp"
#include "Logger.hpp"
#include "TcpSocketFactory.hpp"

namespace webserv {
    class Server {
    public:
        static Socket listen(const std::string &ipAddr,
            const unsigned short port) {
            ISocketFactory* factory = new TcpSocketFactory();
            Socket socket = factory->createSocket(ipAddr, port);
            delete factory;
            return socket;
        }
    };
}
#endif
