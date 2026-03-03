/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Endpoint.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:33:59 by yroard            #+#    #+#             */
/*   Updated: 2026/02/17 11:01:53 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENDPOINT_HPP
#define ENDPOINT_HPP

#include <cstring>
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sstream>
#include <netdb.h>

#include "ConvUtils.hpp"
#include "EndPointError.hpp"


namespace webserv {
    class Socket;

    class Endpoint {
    private:
        const std::string m_addr;
        const unsigned short m_port;
        struct sockaddr_in m_sockaddr_in;
        socklen_t m_sockaddr_len;

        // Use getaddrinfo to resolve the address and port
        // For a server binding, you might pass NULL for 'node' if you want
        // to listen on all interfaces.
        // If you want to bind to a specific IP, pass 'addr.c_str()'.
        // We only care about the first result for this simplified
        // Endpoint, assuming it's valid.
        Endpoint(const std::string &addr, const unsigned short port)
            : m_addr(addr), m_port(port), m_sockaddr_in() {
            struct addrinfo hints, *servinfo;
            int rv;
            memset(&hints, 0, sizeof hints);
            hints.ai_family = AF_INET; // Force IPv4
            hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
            hints.ai_flags = AI_PASSIVE; // Fill in my IP for me (for server bind)
            const char *node = m_addr.c_str(); // Use specific IP
            if (m_addr == "0.0.0.0" || m_addr == "::") {
                node = NULL;
                hints.ai_flags |= AI_PASSIVE; // Ensure AI_PASSIVE for NULL node
            } else {
                hints.ai_flags &= ~AI_PASSIVE; // Clear AI_PASSIVE if specific IP
            }
            if ((rv = getaddrinfo(node,
                ConvUtils::uShortToStr(port).c_str(), &hints,
                                  &servinfo)) != 0) {
                throw EndpointError::create(invalid_address_port,
                                            gai_strerror(rv));
            }
            if (servinfo == NULL) {
                throw EndpointError::create(invalid_address_port,
                                            "getaddrinfo returned no results");
            }
            if (servinfo->ai_family == AF_INET) {
                std::memcpy(&m_sockaddr_in, servinfo->ai_addr, servinfo->ai_addrlen);
                m_sockaddr_len = servinfo->ai_addrlen;
            } else {
                freeaddrinfo(servinfo);
                throw EndpointError::create(invalid_address_port,
                                            "getaddrinfo did not return IPv4 address");
            }
            freeaddrinfo(servinfo);
        }

    public:
        ~Endpoint() {
        };

        static Endpoint createIpv4Addr(const std::string &addr, unsigned short port) {
            return Endpoint(addr, port);
        }

        const char *getIpAddress() const {
            return m_addr.c_str();
        }

        unsigned short getPort() const {
            return m_port;
        }

        struct sockaddr *getSockAddr() {
            return reinterpret_cast<struct sockaddr *>(&m_sockaddr_in);
        }
    };
}

#endif
