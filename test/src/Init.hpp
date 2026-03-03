//
// Created by yannou on 05/02/2026.
//

#ifndef WEBSERV_INIT_HPP
#define WEBSERV_INIT_HPP

#include "Webserv.hpp"

//select the level of logs to print on terminal (DEBUG, INFO,
//WARNING, ERR) and handle timeout (in seconds) with bool stopRequested
namespace webserv {
    struct Init {
        static const LogLevel   logLevel = INFO;
        static const int        timeOutLimit = 30;
        static bool             stopRequested;
    };
}

#endif //WEBSERV_INIT_HPP