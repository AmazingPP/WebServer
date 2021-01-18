//
// Created by AmazingPP on 2021/1/18.
//

#pragma once

#ifndef WEBSERVER_UTILS_H
#define WEBSERVER_UTILS_H

#include "framework.h"

namespace utils {
    bool check_path_vail(const char* base_path);
    void handle_for_sigpipe();
}


#endif //WEBSERVER_UTILS_H
