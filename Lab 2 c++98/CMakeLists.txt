cmake_minimum_required(VERSION 3.30)
project(Lab_2_c__98)

set(CMAKE_CXX_STANDARD 98)

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

add_executable(Lab_2_c__98 main.cpp
        ArrayHandler.cpp
        ArrayHandler.h)
add_executable(Test test.cpp
        ArrayHandler.cpp
        ArrayHandler.h)

target_link_libraries(Test PRIVATE gtest_main)

add_test(NAME MyTest COMMAND Test)

