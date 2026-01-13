/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfigRoutes.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:30:05 by yroard            #+#    #+#             */
/*   Updated: 2025/12/11 09:30:08 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SERVERCONFIGROUTES_HPP
# define SERVERCONFIGROUTES_HPP

#include "IServerConfigRoutes.hpp"
#include "ServiceConfigAutoIndex.hpp"
#include "ServiceConfigCGI.hpp"
#include "ServiceConfigDefaultFile.hpp"
#include "ServiceConfigMethod.hpp"
#include "ServiceConfigRedirection.hpp"
#include "ServiceConfigRootPath.hpp"
#include "ServiceConfigStoreDirectory.hpp"

namespace webserv {
    namespace serverConfig {
        namespace routes{
            class ServerConfigRoutes : public IServerConfigRoutes {
            private:
                const ServiceConfigRouteLoc m_routeLoc;
                const ServiceConfigMethod m_method;
                const ServiceConfigRedirection m_redirection;
                const ServiceConfigRootPath m_rootPath;
                const ServiceConfigAutoIndex m_autoIndex;
                const ServiceConfigDefaultFile m_defaultFile;
                const ServiceConfigStoreDirectory m_storeDirectory;
                const ServiceConfigCGI m_cgi;

            public:
                explicit ServerConfigRoutes(
                    const ServiceConfigRouteLoc &routeLoc,
                    const ServiceConfigMethod &method,
                    const ServiceConfigRedirection &redirection,
                    const ServiceConfigRootPath &rootPath,
                    const ServiceConfigAutoIndex &autoIndex,
                    const ServiceConfigDefaultFile &defaultFile,
                    const ServiceConfigStoreDirectory &storeDirectory,
                    const ServiceConfigCGI &cgi)
                    : m_routeLoc(routeLoc), m_method(method),
                        m_redirection(redirection), m_rootPath(rootPath),
                        m_autoIndex(autoIndex), m_defaultFile(defaultFile),
                        m_storeDirectory(storeDirectory), m_cgi(cgi){}

                virtual ~ServerConfigRoutes(){}

                const ServiceConfigRouteLoc &getRouteLoc()const {
                    return m_routeLoc;
                }

                const ServiceConfigMethod &getMethod()const {
                    return m_method;
                }

                const ServiceConfigRedirection &getRedirection()const {
                    return m_redirection;
                }

                const ServiceConfigRootPath& getRootPath() const {
                    return m_rootPath;
                }

                ServiceConfigAutoIndex getAutoIndex()const {
                    return m_autoIndex;
                }

                const ServiceConfigDefaultFile &getDefaultFile()const {
                    return m_defaultFile;
                }

                const ServiceConfigStoreDirectory &getStoreDirectory()const {
                    return m_storeDirectory;
                }

                const ServiceConfigCGI &getCGI()const {
                    return m_cgi;
                }

            };
        }
    }
}
#endif