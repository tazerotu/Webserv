/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServiceConfigRouteLoc.hpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:31:35 by yroard            #+#    #+#             */
/*   Updated: 2025/12/11 09:31:38 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVICECONFIGROUTELOC_HPP
# define SERVICECONFIGROUTELOC_HPP

#include <string>
#include <cstring>
#include <dirent.h>
#include <cerrno>

#include "../IServerConfigError.hpp"

namespace webserv {
    namespace serverConfig {
        namespace routes{
            class ServiceConfigRouteLoc {
            private:
                const std::string m_routeLoc;
            public:
                explicit ServiceConfigRouteLoc(const std::string& routeLoc)
                    : m_routeLoc(routeLoc) {}
                ~ServiceConfigRouteLoc(){}
                static ServiceConfigRouteLoc create(
                        const std::string& routeLoc){
                    if (routeLoc.empty())
                        throw IServerConfigError::create
                            (invalid_route_Location,NULL);
                    return ServiceConfigRouteLoc(routeLoc);
                }
                const std::string& getValue()const { return m_routeLoc; }
            };
        }
    }
}
#endif
