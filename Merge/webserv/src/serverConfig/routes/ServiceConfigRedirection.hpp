/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServiceConfigRedirection.hpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:30:54 by yroard            #+#    #+#             */
/*   Updated: 2025/12/11 09:38:27 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef SERVICECONFIGREDIRECTION_HPP
# define SERVICECONFIGREDIRECTION_HPP

#include <string>
#include <map>
#include "../IServerConfigError.hpp"
#include "../../http/httpStatusCodeValidator.hpp"

namespace webserv {
    namespace serverConfig {
        namespace routes {
            class ServiceConfigRedirection {
            public:
                typedef std::map<int, std::string> Map;
            private:
                const Map m_redirection;

            public:
                explicit ServiceConfigRedirection(const Map &redirection)
                    : m_redirection(redirection) {}

                ~ServiceConfigRedirection(){}

                static bool isValidRedirection(const Map &redirection) {
                    //check for valid code (is it present into http status?)
                    if (redirection.empty())
                        return true; // No redirection is a valid state
                    for (Map::const_iterator it = redirection.begin();
                            it != redirection.end(); ++it) {
                        const int code = it->first;
                        std::string uri = it->second;
                        if (!HttpStatusCodeValidator::intToEnum(it->first))
                            return false;
                        if (code < 300 || code > 399) //codes must be within this range to make sense
                            return false;
                        // check for valid address (is it present into valid address?)
                        if (uri.empty())
                            return false;
                    }
                    return true;
                }

                static ServiceConfigRedirection create(const Map& redirection){
                    if (!isValidRedirection(redirection))
                        throw IServerConfigError::create(
                                invalid_redirection,NULL);
                    return ServiceConfigRedirection(redirection);
                }

                const Map& getValue()const { return m_redirection; }
            };
        }
    }
}
#endif
