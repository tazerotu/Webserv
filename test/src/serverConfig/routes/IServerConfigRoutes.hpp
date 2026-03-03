/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IServerConfigRoutes.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:29:17 by yroard            #+#    #+#             */
/*   Updated: 2026/01/21 15:48:53 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef ISERVERCONFIGROUTES_HPP
# define ISERVERCONFIGROUTES_HPP

#include "ServiceConfigMethod.hpp"
#include "ServiceConfigRedirection.hpp"
#include "ServiceConfigRootPath.hpp"
#include "ServiceConfigAutoIndex.hpp"
#include "ServiceConfigDefaultFile.hpp"
#include "ServiceConfigStoreStatus.hpp"
#include "ServiceConfigCGI.hpp"
#include "ServiceConfigRouteLoc.hpp"

namespace webserv {
    namespace serverConfig {
        namespace routes{
            class IServerConfigRoutes {
            public:
                virtual ~IServerConfigRoutes(){};
                virtual const ServiceConfigRouteLoc &getRouteLoc() const = 0;
                virtual const ServiceConfigMethod &getMethod() const = 0;
                virtual const ServiceConfigRedirection
                    &getRedirection() const = 0;
                virtual const ServiceConfigRootPath &getRootPath() const= 0;
                virtual ServiceConfigAutoIndex getAutoIndex() const = 0;
                virtual const ServiceConfigDefaultFile &getDefaultFile() const = 0;
                virtual const ServiceConfigStoreStatus
                    &getStoreStatus() const = 0;
                virtual const ServiceConfigCGI &getCGI() const = 0;
            };
        }
    }
}
#endif

