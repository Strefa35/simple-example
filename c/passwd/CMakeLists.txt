cmake_minimum_required(VERSION 3.2)
project(passwd)

set(SOURCE_FILES 
    passwd.c
)

add_executable(passwd ${SOURCE_FILES})
