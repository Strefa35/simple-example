cmake_minimum_required(VERSION 3.2)
project(dns)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(SOURCE_FILES
    dns.cpp
)

add_executable(dns ${SOURCE_FILES})

