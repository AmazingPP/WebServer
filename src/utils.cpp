//
// Created by AmazingPP on 2021/1/18.
//

#include "../include/utils.h"

#include <csignal>
#include <sys/stat.h>

namespace utils {
    std::string &TrimLeft(std::string &str) {
        if (size_t pos = str.find_first_not_of(' '); pos != std::string::npos) {
            str.erase(0,pos);
        }

        return str;
    }

    std::string &TrimRight(std::string &str) {
        if (size_t pos = str.find_last_not_of(' '); pos != std::string::npos) {
            str.erase(pos + 1);
        }

        return str;
    }

    std::string &Trim(std::string &str) {
        TrimLeft(str);
        TrimRight(str);
        return str;
    }

    bool IsPathVaild(const char* base_path){
        struct stat file;

        if (stat(base_path, &file) == -1) {
            return false;
        }
        if (!S_ISDIR(file.st_mode) || access(base_path,R_OK) == -1){
            return false;
        }

        return true;
    }

    void HandleForSigpipe() {
        struct sigaction sa;
        sa.sa_handler = SIG_IGN;
        sa.sa_flags = 0;
        if (sigaction(SIGPIPE, &sa, nullptr))
            return;
    }

    std::pair<std::string, std::string> ParseToKeyValue(const char *buffer) {
        std::string str(buffer);

        if (size_t pos = str.find(':'); pos != std::string::npos) {
            return { str.substr(0, pos), str.substr(pos + 1) };
        }
        else {
            return { "","" };
        }
    }
}
