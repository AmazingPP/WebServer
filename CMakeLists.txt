cmake_minimum_required(VERSION 3.17)
project(WebServer)

set(CMAKE_CXX_STANDARD 17)

add_executable(WebServer src/main.cpp include/framework.h include/noncopyable.h src/utils.cpp include/utils.h include/threading/mutex_lock.h include/threading/condition.h src/sockets/client_socket.cpp include/sockets/client_socket.h)