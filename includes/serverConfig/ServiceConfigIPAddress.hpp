/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServiceConfigIPAddress.hpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:32:38 by yroard            #+#    #+#             */
/*   Updated: 2025/12/11 09:32:41 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVICECONFIGIPADDRESS_HPP
# define SERVICECONFIGIPADDRESS_HPP

#include <string>
#include <vector>
#include <sstream>

namespace webserv {
    namespace serverConfig {
        class ServiceConfigIPAddress {
            const std::string m_iPAddress;

        public:
            // ServiceConfigIPAddress() : m_IPAddress("") {};
            explicit ServiceConfigIPAddress(const std::string &iPAddress)
                : m_iPAddress(iPAddress) {}
            ~ServiceConfigIPAddress(){}
            static bool isValidIPAddress(const std::string &iPAddress) {
                std::stringstream ss(iPAddress);
                std::string segment;
                std::vector<std::string> segList;
                // 1. Split string by '.'
                while (std::getline(ss, segment, '.')) {
                    segList.push_back(segment);
                }
                // 2. Check if we have exactly 4 segments
                if (segList.size() != 4)
                    return false;
                // Also check if the string ends with a dot
                if (iPAddress[iPAddress.size() - 1] == '.')
                    return false;
                for (size_t i = 0; i < segList.size(); ++i) {
                    const std::string& s = segList[i];
                    // 3. Segment cannot be empty
                    if (s.empty())
                        return false;
                    // 4. Check for non-digit characters
                    for (size_t j = 0; j < s.size(); ++j) {
                        if (!isdigit(s[j]))
                            return false;
                    }
                    // 5. Convert to integer
                    int num = 0;
                    std::istringstream converter(s);
                    converter >> num;
                    // 6. Check Range (0-255)
                    if (num < 0 || num > 255)
                        return false;
                    // 7. Check for leading zeros (e.g., "192.168.01.1" is technically invalid/octal in some parsers)
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
