//
// Created by AmazingPP on 2021/1/19.
//

#pragma once

#ifndef WEBSERVER_LOGGER_H
#define WEBSERVER_LOGGER_H

#endif //WEBSERVER_LOGGER_H

namespace logger {
    template<typename... Arg>
    void Info(const char* format, Arg&&... args) {
        std::printf(format, std::forward<Arg>(args)...);
    }

    template<typename... Arg>
    void Error(const char* format, Arg&&... args) {
        std::printf(format, std::forward<Arg>(args)...);
    }
}

#define LOG_ERROR(...) logger::Error((utils::Format(__VA_ARGS__) + utils::Format("[%s] [%s : %d]", strerror(errno), __FILE__, __LINE__)).c_str());