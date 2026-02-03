/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServiceConfigPort.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:33:02 by yroard            #+#    #+#             */
/*   Updated: 2025/12/11 09:33:05 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVICECONFIGPORT_HPP
# define SERVICECONFIGPORT_HPP

#include <limits>
#include "IServerConfigError.hpp"

namespace webserv {
    namespace serverConfig {
        class ServiceConfigPort {
            const unsigned short m_port;

        public:
            // ServiceConfigPort() : m_port(0) {}
            explicit ServiceConfigPort(const unsigned short port): m_port(port) {}
            ~ServiceConfigPort() {}
            static bool isValidPort(int port) {
                if (port < 1 || port > std::numeric_limits<unsigned short>::max())
                    return false;
                return true;
            }
            static ServiceConfigPort create(const int port) {
                if (!isValidPort(port))
                    throw IServerConfigError::create(
                        invalid_port,NULL);
                const unsigned short unShortPort = static_cast<unsigned short>(port);
                return ServiceConfigPort(unShortPort);
            }

            unsigned short getValue() const { return m_port; }
        };
    }
}
#endif
