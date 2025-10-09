cmake_minimum_required(VERSION 3.30)
project(Lab_3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX)

add_executable(Lab_3 main.cpp
        ArrayHandler.cpp
        ArrayHandler.h
        ThreadHandler.cpp
        ThreadHandler.h)
