/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exception.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:28:22 by yroard            #+#    #+#             */
/*   Updated: 2025/12/11 11:38:16 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXCEPTION_HPP
# define EXCEPTION_HPP

#include <string>
#include "HttpStatus.hpp"

namespace webserv {
    namespace http {
        typedef enum eExceptError {
            invalid_method,
            invalid_path,
            invalid_httpVersion,
            invalid_lineRequest,
            invalid_header
        }tExceptError;

//
        class Exception : public std::exception {
        private:
            HttpStatus m_status;
            std::string m_msg;
            Exception(HttpStatus status, const std::string &msg) : m_status(status), m_msg(msg) {
            };

        public:
            ~Exception() throw() {
            };



            static Exception create(const HttpStatus status, const tExceptError error) {
                std::string full_msg = "";
                full_msg += " : ";
                switch (error) {
                    case invalid_method:
                        full_msg = "method in this query is not handled";
                        break;
                    case invalid_path:
                        full_msg = "path is not handled properly";
                        break;
                    case invalid_httpVersion:
                        full_msg = "httpVersion is not handled properly";
                        break;
                    case invalid_lineRequest:
                        full_msg = "line request is not handled properly";
                        break;
                    case invalid_header:
                        full_msg = "header request is not handled properly";
                        break;
                    default:
                        full_msg = "Unknown http error";
                }
                return Exception(status, full_msg);
            }
        };
    }
}
#endif
