cmake_minimum_required(VERSION 4.0)
project(Lab_5)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_executable(Lab_5 src/main.cpp
        src/Employee.cpp
        headers/Employee.h)
