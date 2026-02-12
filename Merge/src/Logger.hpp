//
// Created by yannou on 05/02/2026.
//

#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <ctime>
#include <iomanip>
#include "Webserv.hpp"
#include "Init.hpp"

namespace webserv {
    class Logger {
    private:
        Logger();
        ~Logger();
    public:
        static std::string getCurrentTime() {
            std::time_t now = std::time(NULL);
            std::string timeStr = std::ctime(&now);
			// std::string timeStr = "0";
            timeStr.erase(timeStr.find_last_not_of("\n") + 1); // Remove newline
            return timeStr;
        }

        static void MessagesFilter(LogLevel messLogLevel,
                std::string contextMessage, std::string result) {
            if (messLogLevel < Init::logLevel)
                return;
            std::string levelStr;
            switch (messLogLevel) {
                case DEBUG: levelStr = "DEBUG"; break;
                case INFO: levelStr = "INFO"; break;
                case WARNING: levelStr = "WARNING"; break;
                case ERR: levelStr = "ERROR"; break;
                default: levelStr = "UNKNOWN"; break;
            }
            std::string timeStr = getCurrentTime();
            if (messLogLevel == DEBUG || messLogLevel == INFO) {
                std::cout << "[" << timeStr << "]" << "<" << levelStr << ">"
                    << contextMessage << result << std::endl;
                return;
            }
            std::cerr << "[" << timeStr << "]" << "<" << levelStr << "> "
                    << contextMessage << ": " << result << std::endl;
        }

        // Helper function to filter unprintable characters
        static std::string filterUnprintable(const std::string& input) {
            std::string filtered;
            for (size_t i = 0; i < input.size(); ++i) {
                unsigned char c = input[i];
                if (c >= 32 && c <= 126) {  // Printable ASCII range
                    filtered += c;
                } else if (c == '\r') {
                    filtered += "\\r";
                } else if (c == '\n') {
                    filtered += "\\n\n";  // Add extra newline for readability
                } else {
                    filtered += ".";  // Replace unprintable characters with a dot
                }
            }
            return filtered;
        }
    };
}
#endif