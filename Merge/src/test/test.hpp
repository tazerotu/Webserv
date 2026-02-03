/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yroard <yroard@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:33:38 by yroard            #+#    #+#             */
/*   Updated: 2025/12/11 09:33:42 by yroard           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_HPP
# define TEST_HPP
#include "../serverConfig/IServerConfig.hpp"

namespace webserv {
    namespace test {
        void testIPAddress();
        void testPort();
        void testMaxBodySize();
    }
}


#endif