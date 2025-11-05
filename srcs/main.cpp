/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 09:31:11 by ttas              #+#    #+#             */
/*   Updated: 2025/11/05 13:42:27 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// #include "../includes/Webserv.hpp"

#include <iostream>

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		std::cout << "Too many arguments!" << std::endl << "correct syntax : ./webserv [path/to/conf_file]" << std::endl;
		return(0);	
	}
	
}