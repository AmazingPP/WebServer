//
// Created by AmazingPP on 2021/1/18.
//

#pragma once

#include "../include/utils.h"

#include <signal.h>
#include <sys/stat.h>

namespace utils {
    bool check_path_vail(const char* base_path){
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
        memset(&sa, '\0', sizeof(sa));
        sa.sa_handler = SIG_IGN;
        sa.sa_flags = 0;
        if (sigaction(SIGPIPE, &sa, nullptr))
            return;
    }
}
