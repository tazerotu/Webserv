/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServiceConfigAutoIndex.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:30:14 by yroard            #+#    #+#             */
/*   Updated: 2025/12/11 09:30:18 by yroard           ###   ########.fr       */
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
                // ServiceConfigAutoIndex() : m_port(0) {}
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
