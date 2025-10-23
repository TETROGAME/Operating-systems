cmake_minimum_required(VERSION 4.0)
project(Colloquium)

set(CMAKE_CXX_STANDARD 20)

add_executable(Colloquium main.cpp
        include/factorial.h
        factorial.cpp)
