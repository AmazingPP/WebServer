//
// Created by AmazingPP on 2021/1/19.
//

#pragma once

#ifndef WEBSERVER_LOGGER_H
#define WEBSERVER_LOGGER_H

#endif //WEBSERVER_LOGGER_H

namespace logger {
    template<typename... Arg>
    void info(const char* format, Arg&&... args) {
        std::printf(format, std::forward<Arg>(args)...);
    }

    template<typename... Arg>
    void error(const char* format, Arg&&... args) {
        std::printf(format, std::forward<Arg>(args)...);
    }
}

#define LOG_ERROR(...) logger::error((utils::format(__VA_ARGS__) + utils::format("[%s] [%s : %d]", strerror(errno), __FILE__, __LINE__)).c_str());