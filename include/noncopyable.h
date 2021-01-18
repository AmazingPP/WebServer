//
// Created by AmazingPP on 2021/1/18.
//

#pragma once

#ifndef WEBSERVER_NONCOPYABLE_H
#define WEBSERVER_NONCOPYABLE_H

class noncopyable {
protected:
    noncopyable() = default;
    ~noncopyable() = default;
public:
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
};

#endif //WEBSERVER_NONCOPYABLE_H
