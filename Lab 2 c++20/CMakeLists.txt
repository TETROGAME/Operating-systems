cmake_minimum_required(VERSION 3.30)
project(Lab_2_c__20)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebugDLL" CACHE STRING "" FORCE)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(Lab_2_c__20 main.cpp
        ArrayHandler.cpp
        ArrayHandler.h
        ThreadHandler.cpp
        ThreadHandler.h
        Holder.cpp
        Holder.h)
add_executable(Test test.cpp
        ArrayHandler.cpp
        ArrayHandler.h
        ThreadHandler.cpp
        ThreadHandler.h
        Holder.cpp
        Holder.h)

target_link_libraries(Test PRIVATE gtest_main)

add_test(NAME MyTest COMMAND Test)

