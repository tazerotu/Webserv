/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:32:20 by yroard            #+#    #+#             */
/*   Updated: 2025/12/11 09:32:24 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

#include "IServerConfig.hpp"


namespace webserv {
    namespace serverConfig {
        class ServerConfig : public IServerConfig {
        private:
            const ServiceConfigWebsiteName m_websiteName;
            const ServiceConfigIPAddress m_iPAddress;
            const ServiceConfigPort m_port;
            ServiceConfigErrorPages m_errorPages;
            const ServiceConfigMaxBodySize m_maxBodySize;
            const TabRoute m_routes;
        public:
            explicit ServerConfig(const ServiceConfigWebsiteName &websiteName,
                const ServiceConfigIPAddress &iPAddress,
                const ServiceConfigPort &port,
                const ServiceConfigErrorPages &errorPages,
                const ServiceConfigMaxBodySize &maxBodySize,
                const TabRoute &routes)
                : m_websiteName(websiteName), m_iPAddress(iPAddress),
                    m_port(port), m_errorPages(errorPages),
                    m_maxBodySize(maxBodySize), m_routes(routes){}

            virtual ~ServerConfig(){
                for (TabRoute::const_iterator it = m_routes.begin(); it != m_routes.end();
                    ++it)
                    delete *it; // Delete the Route objects
            }

            const ServiceConfigWebsiteName &getWebsiteName()const {return m_websiteName;}

            const ServiceConfigIPAddress &getIPAddress()const {return m_iPAddress;}

            ServiceConfigPort getPort()const {return m_port;}

            const ServiceConfigErrorPages &getErrorPages()const {
                return m_errorPages;
            }

            ServiceConfigMaxBodySize getMaxBodySize()const {return m_maxBodySize;}

            const TabRoute& getRoutes()const { return m_routes; }

        };
    }
}
#endif