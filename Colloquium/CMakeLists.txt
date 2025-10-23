cmake_minimum_required(VERSION 4.0)
project(Colloquium)

set(CMAKE_CXX_STANDARD 20)

add_executable(Colloquium main.cpp
        include/factorial.h
        factorial.cpp
        delete_duplicates.cpp
        include/delete_duplicates.h
        linked_list.cpp
        include/linked_list.h)
