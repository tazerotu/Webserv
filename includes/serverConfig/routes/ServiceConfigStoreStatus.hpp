/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServiceConfigStoreStatus.hpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:31:43 by yroard            #+#    #+#             */
/*   Updated: 2026/01/27 12:05:09 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVICECONFIGSTORESTATUS_HPP
# define SERVICECONFIGSTORESTATUS_HPP

#include <string>
#include <dirent.h>
#include "../IServerConfigError.hpp"

namespace webserv {
	namespace serverConfig {
		namespace routes {
			class ServiceConfigStoreStatus {
			private:
				const tStoreStatus m_storeStatus;

			public:
				explicit ServiceConfigStoreStatus(
					const tStoreStatus &storeStatus)
					: m_storeStatus(storeStatus) {}

				~ServiceConfigStoreStatus(){}

				static tStoreStatus isValid(const std::string & storeStatus){
					if (storeStatus == "OFF")
						return OFF;
					if (storeStatus == "ON")
						return ON;
					return ERROR;
				}
				
				static ServiceConfigStoreStatus create(
						const std::string & storeStatus) {
					tStoreStatus value = isValid(storeStatus);
					if (value == ERROR)
						throw IServerConfigError::create(
								invalid_store_status, NULL);
					return ServiceConfigStoreStatus(value);
				}

				const tStoreStatus& getValue()const {
					return m_storeStatus;
				}
			};
		}
	}
}
#endif

