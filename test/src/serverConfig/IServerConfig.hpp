/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IServerConfig.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:31:52 by yroard            #+#    #+#             */
/*   Updated: 2025/12/11 09:31:55 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ISERVERCONFIG_HPP
# define ISERVERCONFIG_HPP


#include "ServiceConfigWebsiteName.hpp"
#include "ServiceConfigIPAddress.hpp"
#include "ServiceConfigPort.hpp"
#include "ServiceConfigErrorPages.hpp"
#include "ServiceConfigMaxBodySize.hpp"
#include "routes/ServerConfigRoutes.hpp"

namespace webserv {
    namespace serverConfig {
        typedef std::vector<routes::IServerConfigRoutes*> TabRoute;
        class IServerConfig {
        public:
            virtual ~IServerConfig(){};

            virtual const ServiceConfigWebsiteName &getWebsiteName() const = 0;
            virtual const ServiceConfigIPAddress &getIPAddress() const = 0;
            virtual ServiceConfigPort getPort() const = 0;
            virtual const ServiceConfigErrorPages &getErrorPages() const = 0;
            virtual ServiceConfigMaxBodySize getMaxBodySize() const = 0;
            virtual const TabRoute &getRoutes() const = 0;
        };
    }
}
#endif

