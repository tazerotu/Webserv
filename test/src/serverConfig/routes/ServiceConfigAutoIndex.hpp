/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServiceConfigAutoIndex.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:30:14 by yroard            #+#    #+#             */
/*   Updated: 2026/02/17 09:52:39 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVICECONFIGAUTOINDEX_HPP
# define SERVICECONFIGAUTOINDEX_HPP

// #include <limits>
#include "../IServerConfigError.hpp"

namespace webserv {
    namespace serverConfig {
        namespace routes {
            class ServiceConfigAutoIndex {
                const bool m_autoIndex;

            public:
                explicit ServiceConfigAutoIndex(const bool autoIndex): m_autoIndex(autoIndex) {}
                ~ServiceConfigAutoIndex() {}
                static bool isValidAutoIndex(const int autoIndex) {
                    if (autoIndex < 0 || autoIndex > 1)
                        return false;
                    return true;
                }
                static ServiceConfigAutoIndex create(int autoIndex) {
                    if (!isValidAutoIndex(autoIndex))
                        throw IServerConfigError::create(
                            invalid_auto_index,NULL);
                    return ServiceConfigAutoIndex(static_cast<bool>(autoIndex));
                }

                unsigned short getValue() const {
                    return m_autoIndex;
                }
            };
        }
    }
}
#endif
