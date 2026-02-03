/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServiceConfigMaxBodySize.hpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:32:47 by yroard            #+#    #+#             */
/*   Updated: 2025/12/11 09:32:54 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVICECONFIGMAXBODYSIZE_HPP
# define SERVICECONFIGMAXBODYSIZE_HPP

#include "IServerConfigError.hpp"

namespace webserv {
    namespace serverConfig {
        class ServiceConfigMaxBodySize {
            const unsigned m_maxBodySize;

        public:
            // ServiceConfigMaxBodySize(): m_maxBodySize(0) {}
            ServiceConfigMaxBodySize(const unsigned maxBodySize)
            : m_maxBodySize(maxBodySize) {}

            ~ServiceConfigMaxBodySize() {}

            static bool isValidMaxBodySize(int maxBodySize) {
                if (maxBodySize < 0)
                    return false;
                return true;
            }

            static ServiceConfigMaxBodySize create(const int maxBodySize) {
                if (!isValidMaxBodySize(maxBodySize)) {
                    throw IServerConfigError::create(
                        invalid_body_max_size, NULL);
                }
                return ServiceConfigMaxBodySize(maxBodySize);
            }

            const unsigned& getValue() const { return m_maxBodySize; }
        };
    }
}
#endif
