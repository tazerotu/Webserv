/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MethodFactory.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:46:51 by yroard            #+#    #+#             */
/*   Updated: 2026/02/17 15:49:31 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef METHODFACTORY_HPP
#define METHODFACTORY_HPP

#include <map>
#include <string>
#include "Method.hpp"

namespace webserv{
    namespace http{
        class MethodFactory {
        public:
            typedef std::map<std::string, tVerb> Map;

        private:
            static const Map map;

            static Map init_map();

        public:
            static Method createMethod(const std::string &method) {
                const Map::const_iterator it = map.find(method);
                if (it == map.end()) {
                    throw Exception::create(HttpStatus(MethodNotAllowed),
                                            invalid_method);
                }
                return Method(it->second);
            }
        };
    }
}
