//
// Created by AmazingPP on 2021/1/18.
//

#pragma once

#ifndef WEBSERVER_UTILS_H
#define WEBSERVER_UTILS_H

#include "framework.h"

namespace utils {
    template<typename... Arg>
    std::string format(const char* format, Arg&&... args) {
        char buffer[1024]{};
        snprintf(buffer, sizeof(buffer) - 1, format, std::forward<Arg>(args)...);
        return std::string(buffer);
    }

    std::string& ltrim(std::string &str);
    std::string& rtrim(std::string &str);
    std::string& trim(std::string &str);
    bool check_path_vaild(const char* base_path);
    void handle_for_sigpipe();
}


#endif //WEBSERVER_UTILS_H
