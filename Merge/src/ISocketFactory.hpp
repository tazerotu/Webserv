//
// Created by yannou on 06/02/2026.
//

#ifndef WEBSERV_ISOCKETFACTORY_HPP
#define WEBSERV_ISOCKETFACTORY_HPP

#include "Endpoint.hpp"

namespace webserv {
    class ISocketFactory {
    public:
        virtual ~ISocketFactory() {}
        virtual Socket createSocket(const std::string &ipAddr,
            const unsigned short port) const = 0;
    };
}


#endif //WEBSERV_ISOCKETFACTORY_HPP