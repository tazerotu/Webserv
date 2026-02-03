/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by nou               #+#    #+#             */
/*   Updated: 2025/12/11 09:27:47 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
# define METHOD_HPP

#include <map>
#include <string>
#include "Exceptions.hpp"
#include "HttpStatus.hpp"
#include "../Webserv.hpp"

namespace webserv {
    namespace http {
        class MethodFactory;

        class Method {
        private:
            const tVerb m_method;

            explicit Method(const tVerb method) : m_method(method) {
            }

        public:
            std::string toString() {
                switch (m_method) {
                    case GET:
                        return "GET";
                    case POST:
                        return "POST";
                    case DELETE:
                        return "DELETE";
                    default:
                        return "";
                }
            }

            friend MethodFactory;
        };

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

#endif
*/
