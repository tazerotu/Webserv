// #ifndef SCMETHODFACTORY_HPP
// # define SCMETHODFACTORY_HPP

// #include <string>
// #include <map>
// #include <vector>
// #include "ServiceConfigMethod.hpp"
// #include "../IServerConfigError.hpp"


// namespace webserv{
//     namespace serverConfig{
//         namespace routes{           
// 			class SCMethodFactory {
// 			public:
// 				typedef std::map<std::string, Verb> Map;

// 			private:
// 				static const Map map;
// 				static Map init_map();

// 			public:
// 				static ServiceConfigMethod createMethod(
// 						const std::string &rawMethods) {
// 					std::vector<webserv::Verb> methodsVec;
// 					std::stringstream ss(rawMethods);
// 					std::string token;
// 					while (ss >> token) {
// 						const Map::const_iterator it = map.find(token);
// 						if (it == map.end()) {
// 							throw IServerConfigError::create(invalid_method, NULL);
// 						}
// 						methodsVec.push_back(it->second);
// 					}
// 					return ServiceConfigMethod(methodsVec);
// 				}
// 			};
//         }
//     }
// }
// #endif