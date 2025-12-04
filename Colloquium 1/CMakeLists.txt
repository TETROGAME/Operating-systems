cmake_minimum_required(VERSION 4.0)
project(Colloquium)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Colloquium main.cpp
        include/factorial.h
        factorial.cpp
        delete_duplicates.cpp
        include/delete_duplicates.h
        linked_list.cpp
        include/linked_list.h)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(unit_tests
        tests/test_factorial.cpp
        tests/test_delete_duplicates.cpp
        tests/test_linked_list.cpp
        factorial.cpp
        delete_duplicates.cpp
        linked_list.cpp
        include/factorial.h
        include/delete_duplicates.h
        include/linked_list.h
        tests/test_factorial.cpp
        tests/test_delete_duplicates.cpp
        tests/test_linked_list.cpp
)

target_link_libraries(unit_tests
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)