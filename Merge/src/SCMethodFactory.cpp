/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SCMethodFactory.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:29:27 by yroard            #+#    #+#             */
/*   Updated: 2026/02/03 10:47:34 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./serverConfig/routes/ServiceConfigMethod.hpp"

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
