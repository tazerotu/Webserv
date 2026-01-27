/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   IServerConfigError.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:32:00 by yroard            #+#    #+#             */
/*   Updated: 2026/01/21 15:35:22 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIGINTERFACEERROR_HPP
# define SERVERCONFIGINTERFACEERROR_HPP

#include <string>

namespace webserv {
	namespace serverConfig {
		typedef enum eServConfError {
			invalid_route_Location,
			invalid_server_name,
			invalid_address,
			invalid_port,
			invalid_error_page,
			invalid_error_directory,
			invalid_body_max_size,
			invalid_method,
			invalid_redirection,
			invalid_root_path,
			invalid_auto_index,
			invalid_default_file,
			invalid_store_status,
			invalid_CGI,
			Invalid_CGI_interpreter
		}tServConfError ;

		class IServerConfigError : public std::exception {
		private:
			std::string m_msg;
			explicit IServerConfigError(const std::string &msg) : m_msg(msg) {
			};
			IServerConfigError& operator=(const IServerConfigError&);

		public:
			IServerConfigError(const IServerConfigError& copy) {
				m_msg = copy.m_msg;
			}
			~IServerConfigError() throw() {
			};

			static IServerConfigError create(const tServConfError error,
					const char *detail = NULL) {
				std::string full_msg;
				switch (error) {
					case invalid_route_Location:
						full_msg = "Location path can not be empty";
						break;
					case invalid_server_name:
						full_msg = "Server name is not valid";
						break;
					case invalid_address:
						full_msg = "address is not valid";
						break;
					case invalid_port:
						full_msg = "port is not valid";
						break;
					case invalid_method:
						full_msg = "method is not valid";
						break;
					case invalid_redirection:
						full_msg = "redirection is not valid: check error code and/or redirection address";
						break;
					case invalid_error_page:
						full_msg = "error page is not valid: check error code";
						break;
					case invalid_error_directory:
						full_msg = "error page is not valid: check address ";
						break;
					case invalid_body_max_size:
						full_msg = "body max size value is not valid";
						break;
					case invalid_root_path:
						full_msg = "root path is not valid";
						break;
					case invalid_auto_index:
						full_msg = "auto index value is not valid";
						break;
					case invalid_default_file:
						full_msg = "default file does not exist";
						break;
					case invalid_store_status:
						full_msg = "status for storing is not valid (ON or OFF)";
						break;
					case invalid_CGI:
						full_msg = "CGI path or extension is missing";
						break;
					case Invalid_CGI_interpreter:
						full_msg = "CGI extension is not found or is not executable";
						break;	

					default:
						full_msg = "Unknown server config error";
				}
				if (detail) {
					full_msg += ": ";
					full_msg += detail;
				}
				return IServerConfigError(full_msg);
			}

			const char *what() const throw() {
				return m_msg.c_str();
			}
		};
	}
}

#endif
