/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 13:31:12 by ttas              #+#    #+#             */
/*   Updated: 2025/11/05 13:57:36 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ServerConfig.hpp"

ServerConfig::ServerConfig()
{

}

ServerConfig::ServerConfig(std::string conf)
{
	std::ifstream file(conf);
	if(!file.is_open())
	{
		std::cout << "Empty of non existant config file" << std::endl;
		exit(0);
	}
}




std::string const &getName(void);
std::string const &getHost(void);
int                getPort(void);
std::string const &getRoot(void);
std::string const &getIndex(void);
std::string const &getErrorPage(void);
std::string const *getAllowedMethods(void);
int                getMaxClientBodySize(void);
std::string const &getCgiPath(void);
std::string const &getCgiExt(void);