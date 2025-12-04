cmake_minimum_required(VERSION 3.11)
project(NamedPipesLab)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(headers)

set(COMMON_SOURCES
        src/Employee.cpp
        src/Request.cpp
        src/FileHandler.cpp
)

add_executable(server src/server.cpp ${COMMON_SOURCES})
add_executable(client src/client.cpp ${COMMON_SOURCES})

enable_testing()

file(GLOB TEST_SOURCES "tests/*.cpp")

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE} ${COMMON_SOURCES})
    target_link_libraries(${TEST_NAME} gtest gtest_main)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()