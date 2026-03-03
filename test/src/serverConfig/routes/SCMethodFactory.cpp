/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SCMethodFactory.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:29:27 by yroard            #+#    #+#             */
/*   Updated: 2026/02/17 15:41:35 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServiceConfigMethod.hpp"

namespace webserv {
    namespace serverConfig {
        namespace routes {
            SCMethodFactory::Map SCMethodFactory::init_map() {
                Map m;
                m.insert(std::make_pair(std::string("GET"), GET));
                m.insert(std::make_pair(std::string("POST"), POST));
                m.insert(std::make_pair(std::string("DELETE"), DELETE));
                return m;
            }

            const SCMethodFactory::Map SCMethodFactory::map = SCMethodFactory::init_map();
        }
    }
}
