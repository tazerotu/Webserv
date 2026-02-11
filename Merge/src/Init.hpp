//
// Created by yannou on 05/02/2026.
//

#ifndef WEBSERV_INIT_HPP
#define WEBSERV_INIT_HPP

#include "Webserv.hpp"

namespace webserv {
    struct Init {
        //select the level of logs to print on terminal
        static const LogLevel logLevel = DEBUG;
        static const int TimeOutLimit = 10;
    };
}

#endif //WEBSERV_INIT_HPP