/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServiceConfigWebsiteName.hpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:33:10 by yroard            #+#    #+#             */
/*   Updated: 2025/12/11 09:33:22 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVICECONFIGSERVERNAME_HPP
# define SERVICECONFIGSERVERNAME_HPP

#include <string>

#include "IServerConfigError.hpp"

namespace webserv {
    namespace serverConfig {
        class ServiceConfigWebsiteName {
            const std::string m_websiteName;

        public:
            explicit ServiceConfigWebsiteName(const std::string& websiteName) : m_websiteName(websiteName) {}
            ~ServiceConfigWebsiteName(){}
            static bool isValidWebsiteName(const std::string &websiteName) {
                if (websiteName.empty())
                    return false;
                //check for valid server Name?
                return true;
            }

            static ServiceConfigWebsiteName create(std::string websiteName) {
                if (!isValidWebsiteName(websiteName))
                    throw IServerConfigError::create(
                        invalid_server_name, NULL);
                return ServiceConfigWebsiteName(websiteName);
            }

            const std::string& getValue()const  {
                return m_websiteName;
            }
        };
    }
}
#endif
