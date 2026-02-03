/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   EndPointError.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:34:09 by yroard            #+#    #+#             */
/*   Updated: 2025/12/11 09:34:11 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENDPOINTERROR_HPP
# define ENDPOINTERROR_HPP

#include <string>

namespace webserv {
    typedef enum eEndPointError {
        invalid_socket,
        invalid_address_port,
        invalid_bind,
        invalid_listen,
        invalid_accept,
        invalid_msg_reception,
        invalid_request
    }tEndPointError;

    class EndpointError : public std::exception {
    private:
        std::string m_msg;

        EndpointError(const std::string &msg) : m_msg(msg) {
        };

    public:
        ~EndpointError() throw() {
        };

        static EndpointError create(const tEndPointError error, const char *detail = NULL) {
            std::string full_msg;
            switch (error) {
                case invalid_socket:
                    full_msg = "Socket creation failed";
                    break;
                case invalid_address_port:
                    full_msg = "Invalid Ipv4 address or port";
                    break;
                case invalid_bind:
                    full_msg = "Bind failed";
                    break;
                case invalid_listen:
                    full_msg = "Listen failed";
                    break;
                case invalid_accept:
                    full_msg = "Accept failed";
                    break;
                case invalid_msg_reception:
                    full_msg = "Message reception failed";
                    break;
                case invalid_request:
                    full_msg = "Message request is not an allowed method";
                    break;
                default:
                    full_msg = "Unknown webserv error";
            }
            if (detail) {
                full_msg += ": ";
                full_msg += detail;
            }
            return EndpointError(full_msg);
        }

        const char *what() const throw() {
            return m_msg.c_str();
        }
    };
}

#endif
