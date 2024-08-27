cmake_minimum_required(VERSION 3.28)
project(cppJson)

set(CMAKE_CXX_STANDARD 23)

add_executable(cppJson main.cpp
    print.h
    json_type.h
    json_type.cpp
    json_parser.cpp
    json_parser.h
    json_err.h
    json_err.cpp
)
