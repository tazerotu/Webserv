/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServiceConfigStoreStatus.hpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:31:43 by yroard            #+#    #+#             */
/*   Updated: 2026/02/11 11:14:22 by ttas             ###   ########.fr       */
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
				const StoreStatus m_storeStatus;

			public:
				explicit ServiceConfigStoreStatus(
					const StoreStatus &storeStatus)
					: m_storeStatus(storeStatus) {}

				~ServiceConfigStoreStatus(){}

				static StoreStatus isValid(const std::string & storeStatus){
					if (storeStatus == "0")
						return OFF;
					if (storeStatus == "1")
						return ON;
					return ERROR;
				}
				
				static ServiceConfigStoreStatus create(
						const std::string & storeStatus) {
					StoreStatus value = isValid(storeStatus);
					if (value == ERROR)
						throw IServerConfigError::create(
								invalid_store_status, NULL);
					return ServiceConfigStoreStatus(value);
				}

				const StoreStatus& getValue()const {
					return m_storeStatus;
				}
			};
		}
	}
}
#endif

