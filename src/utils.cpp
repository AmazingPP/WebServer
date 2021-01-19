//
// Created by AmazingPP on 2021/1/18.
//

#pragma once

#include "../include/utils.h"

#include <csignal>
#include <sys/stat.h>

namespace utils {
    std::string &ltrim(std::string &str) {
        if (size_t pos = str.find_first_not_of(' '); pos != std::string::npos) {
            str.erase(0,pos);
        }

        return str;
    }

    std::string &rtrim(std::string &str) {
        if (size_t pos = str.find_last_not_of(' '); pos != std::string::npos) {
            str.erase(pos + 1);
        }

        return str;
    }

    std::string &trim(std::string &str) {
        ltrim(str);
        rtrim(str);
        return str;
    }

    bool check_path_vaild(const char* base_path){
        struct stat file;

        if (stat(base_path, &file) == -1) {
            return false;
        }
        if (!S_ISDIR(file.st_mode) || access(base_path,R_OK) == -1){
            return false;
        }

        return true;
    }

    void handle_for_sigpipe() {
        struct sigaction sa;
        sa.sa_handler = SIG_IGN;
        sa.sa_flags = 0;
        if (sigaction(SIGPIPE, &sa, nullptr))
            return;
    }
}
