/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServiceConfigMethod.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ttas <ttas@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 09:30:46 by yroard            #+#    #+#             */
/*   Updated: 2026/02/17 10:38:57 by ttas             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#ifndef SERVICECONFIGMETHOD_HPP
# define SERVICECONFIGMETHOD_HPP

#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include "../IServerConfigError.hpp"
#include "../../Webserv.hpp"

namespace webserv {
	namespace serverConfig {
		namespace routes {
			class SCMethodFactory;

			class MethodFactory;

			class ServiceConfigMethod {
			private:
				const std::vector<Verb> m_allowedMethod;

				explicit ServiceConfigMethod(const std::vector<Verb> allowedMethod)
					: m_allowedMethod(allowedMethod){}

			public:
				bool isAllowed(const std::string &methodStr) const {
					Verb verb;
					if (methodStr == "GET") verb = GET;
					else if (methodStr == "POST") verb = POST;
					else if (methodStr == "DELETE") verb = DELETE;
					else return false;

					for (size_t i = 0; i < m_allowedMethod.size(); ++i) {
						if (m_allowedMethod[i] == verb)
							return true;
					}
					return false;
				}
				
				std::string getValue() const {
					std::string result = "";
					for (size_t i = 0; i < m_allowedMethod.size(); ++i) {
						switch (m_allowedMethod[i]) {
							case GET: result += "GET"; break;
							case POST: result += "POST"; break;
							case DELETE: result += "DELETE"; break;
							default: break;
						}
					}
					return result;
				}
				
				friend class SCMethodFactory;
			};

			class SCMethodFactory {
			public:
				typedef std::map<std::string, Verb> Map;

			private:
				static const Map map;
				static Map init_map();

			public:
				static ServiceConfigMethod createMethod(
						const std::string &rawMethods) {
					std::vector<webserv::Verb> methodsVec;
					std::stringstream ss(rawMethods);
					std::string token;
					while (ss >> token) {
						const Map::const_iterator it = map.find(token);
						if (it == map.end()) {
							throw IServerConfigError::create(invalid_method, NULL);
						}
						methodsVec.push_back(it->second);
					}
					return ServiceConfigMethod(methodsVec);
				}

			};
		}
	}
}

#endif
