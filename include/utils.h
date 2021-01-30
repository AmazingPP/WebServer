//
// Created by AmazingPP on 2021/1/18.
//

#ifndef WEBSERVER_UTILS_H
#define WEBSERVER_UTILS_H

#include "framework.h"

namespace utils {
    
    std::string& TrimLeft(std::string &str);

    std::string& TrimRight(std::string &str);

    std::string& Trim(std::string &str);

    std::pair<std::string,std::string> ParseToKeyValue(const char *buffer);

    bool IsPathVaild(const char* base_path);

    void HandleForSigpipe();
}


#endif //WEBSERVER_UTILS_H
