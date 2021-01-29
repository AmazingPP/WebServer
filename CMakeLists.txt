cmake_minimum_required(VERSION 3.17)
project(WebServer)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES
        src/main.cpp
        src/utils.cpp
        src/timer.cpp
        src/epoll.cpp
        src/thread_pool.cpp
        src/sockets/client_socket.cpp
        src/sockets/server_socket.cpp
        src/http/http_request.cpp
        src/http/http_response.cpp
        src/http/http_request_parser.cpp
        src/http/http_data.cpp
        src/http/http_server.cpp
        )

add_executable(WebServer ${SOURCE_FILES})
target_link_libraries(WebServer pthread)

add_subdirectory(vendor/fmt-7.1.3)
target_link_libraries(WebServer fmt::fmt)