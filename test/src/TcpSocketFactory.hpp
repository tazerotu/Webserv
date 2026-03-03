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
                const Endpoint endpoint =
                    Endpoint::createIpv4Addr(ipAddr, port);
                const Socket s(Socket::createTcpSocket());
                s.listenSocket(endpoint);
                return s;
            }
            catch (const std::exception &e) {
                Logger::messagesFilter(ERR,
                    "Failed to listen on ", returnStr);
                Logger::messagesFilter(ERR, 
                    "", e.what());
                throw;
            }
        }
    };
}

#endif //WEBSERV_TCPSOCKETFACTORY_HPP