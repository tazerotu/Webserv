//
// Created by yannou on 30/01/2026.
//

#ifndef WEBSERV_AUTOINDEX_HPP
#define WEBSERV_AUTOINDEX_HPP

#include <cerrno>
#include <cstdio>
#include <dirent.h>
#include <string>

namespace webserv {
    namespace http {
        class AutoIndex {
        public:
            static std::string generateDirContent(const std::string& path) {
                DIR *resultOpenDir = opendir(path.c_str());
                if (!resultOpenDir){
                    perror("open directory");
                    return "";
                }
                std::string result;
                dirent* de;
                while (true){
                    de = readdir(resultOpenDir);
                    if (de == NULL)
                        break;
                    result += std::string(de->d_name);
                    result += "\n";
                }
                closedir(resultOpenDir);
                return result;
            }
        };
    }
}
#endif //WEBSERV_AUTOINDEX_HPP