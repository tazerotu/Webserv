/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Method.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by nou               #+#    #+#             */
/*   Updated: 2026/02/17 15:45:58 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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
    }
}

#endif

