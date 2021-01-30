//
// Created by AmazingPP on 2021/1/19.
//

#ifndef WEBSERVER_LOGGER_H
#define WEBSERVER_LOGGER_H

#include "fmt/os.h"
#include "fmt/color.h"

class Logger {
public:
    template<typename... Arg>
    static void Info(const char *format, Arg&&... args) {
        fmt::print(format, std::forward<Arg>(args)...);
    }

    template<typename... Arg>
    static void Error(const char *format, Arg&&... args) {
        fmt::print(fg(fmt::color::crimson) | fmt::emphasis::bold,
                   format, std::forward<Arg>(args)...);
    }

    template<typename... Arg>
    static void Error(const char *err_str, const char *file, int line, const char *format, Arg&&... args) {
        auto format_str = fmt::format(format, std::forward<Arg>(args)...);
        fmt::print(fg(fmt::color::crimson) | fmt::emphasis::bold,
                   "{} [{}] [{} : {}]", format_str, err_str, file, line);
    }

    template<typename... Arg>
    static void Warning(const char *format, Arg&&... args) {
        fmt::print(fg(fmt::color::yellow) | fmt::emphasis::bold,
                   format, std::forward<Arg>(args)...);
    }
};

#define LOG_ERROR(...) Logger::Error(strerror(errno) , __FILE__, __LINE__, __VA_ARGS__)

#endif //WEBSERVER_LOGGER_H
