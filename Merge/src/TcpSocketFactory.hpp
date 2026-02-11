//
// Created by yannou on 06/02/2026.
//

#ifndef WEBSERV_TCPSOCKETFACTORY_HPP
#define WEBSERV_TCPSOCKETFACTORY_HPP

#include "ISocketFactory.hpp"
#include "Socket.hpp"

namespace webserv {
    class TcpSocketFactory : public webserv::ISocketFactory {
    public:
        Socket createSocket(const std::string &ipAddr,
                const unsigned short port) const {
            std::string returnStr = ipAddr + ":" + ConvUtils::intToStr(port);
            try {
                // 1. Create the Endpoint object, which holds the address info
                const Endpoint endpoint =
                    Endpoint::createIpv4Addr(ipAddr, port);
                // 2. Create the Socket object
                const Socket s(Socket::createTcpSocket());
                // 3. Listen on the socket using the endpoint information
                s.listenSocket(endpoint);
                return s;
            }
            catch (const std::exception &e) {
                Logger::MessagesFilter(ERR,
                    "Failed to listen on ", returnStr);
                Logger::MessagesFilter(ERR, 
                    "", e.what());
                throw;
            }
        }
    };
}

#endif //WEBSERV_TCPSOCKETFACTORY_HPP