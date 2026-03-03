/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServiceConfigIPAddress.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:32:38 by yroard            #+#    #+#             */
/*   Updated: 2026/02/17 09:54:14 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVICECONFIGIPADDRESS_HPP
# define SERVICECONFIGIPADDRESS_HPP

#include <string>
#include <vector>
#include <sstream>
#include "../ConvUtils.hpp"

namespace webserv {
    namespace serverConfig {
        class ServiceConfigIPAddress {
            const std::string m_iPAddress;

        public:
            explicit ServiceConfigIPAddress(const std::string &iPAddress)
                : m_iPAddress(iPAddress) {}
            ~ServiceConfigIPAddress(){}
            static bool isValidIPAddress(const std::string &iPAddress) {
                std::stringstream ss(iPAddress);
                std::string segment;
                std::vector<std::string> segList;
                while (std::getline(ss, segment, '.')) {
                    segList.push_back(segment);
                }
                if (segList.size() != 4)
                    return false;
                if (iPAddress[iPAddress.size() - 1] == '.')
                    return false;
                for (size_t i = 0; i < segList.size(); ++i) {
                    const std::string& s = segList[i];
                    if (s.empty())
                        return false;
                    for (size_t j = 0; j < s.size(); ++j) {
                        if (!isdigit(s[j]))
                            return false;
                    }
                    int num = ConvUtils::cStringToInt(s);
                    if (num < 0 || num > 255)
                        return false;
                    if (s.size() > 1 && s[0] == '0')
                        return false;
                }
                return true;
            }

            static ServiceConfigIPAddress create(const std::string& iPAddress) {
                if (!isValidIPAddress(iPAddress))
                    throw IServerConfigError::create(
                        invalid_address,NULL);
                return ServiceConfigIPAddress(iPAddress);
            }

            const std::string& getValue()const  {
                return m_iPAddress;
            }
        };
    }
}
#endif
