cmake_minimum_required(VERSION 3.30)
project(Lab_3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX)

add_executable(Lab_3 main.cpp
        ThreadHandler.cpp
        ThreadHandler.h
        Array.cpp
        Array.h
        Marker.cpp
        Marker.h)
