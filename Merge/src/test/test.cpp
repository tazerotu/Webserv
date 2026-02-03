/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:33:30 by yroard            #+#    #+#             */
/*   Updated: 2025/12/11 09:33:32 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cassert>
#include "test.hpp"
#include "../serverConfig/ServiceConfigIPAddress.hpp"
#include "../serverConfig/ServiceConfigPort.hpp"
#include "../serverConfig/ServiceConfigMaxBodySize.hpp"

namespace webserv {
    namespace test {
        void testIPAddress() {
            typedef serverConfig::ServiceConfigIPAddress IPAddress;
            assert(IPAddress::isValidIPAddress("192.192.192.192") == true);
            assert(IPAddress::isValidIPAddress("192.192.192.192.") == false);
            assert(IPAddress::isValidIPAddress("002.192.192.192") == false);
            assert(IPAddress::isValidIPAddress("192.192.192.0") == true);
            assert(IPAddress::isValidIPAddress("0.000.192.0") == false);
            assert(IPAddress::isValidIPAddress("0.000.192.0000") == false);
            assert(IPAddress::isValidIPAddress("0,000.192.0000") == false);
            assert(IPAddress::isValidIPAddress("0.ola.192.0") == false);
        }

        void testPort() {
            typedef serverConfig::ServiceConfigPort Port;
            assert(Port::isValidPort(-100) == false);
            assert(Port::isValidPort(0) == false);
            assert(Port::isValidPort(1) == true);
            assert(Port::isValidPort(1000000) == false);
            assert(Port::isValidPort(65535) == true);
            assert(Port::isValidPort(65536) == false);
        }

        void testMaxBodySize() {
            typedef serverConfig::ServiceConfigMaxBodySize MaxBodySize;
            assert(MaxBodySize::isValidMaxBodySize(-100) == false);
            assert(MaxBodySize::isValidMaxBodySize(0) == true);
            assert(MaxBodySize::isValidMaxBodySize(1) == true);
            assert(MaxBodySize::isValidMaxBodySize(1000000) == true);
            assert(MaxBodySize::isValidMaxBodySize(65535) == true);
        }
    }
}