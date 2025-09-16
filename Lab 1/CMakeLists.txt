cmake_minimum_required(VERSION 3.8)

project("Lab 1")

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

add_executable(Main Main.cpp employee.cpp)
add_executable(Creator Creator.cpp employee.cpp)
add_executable(Reporter Reporter.cpp employee.cpp)
add_executable(my_test test.cpp employee.cpp)
target_link_libraries(my_test gtest_main)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_target_properties(Creator Reporter Main my_test PROPERTIES CXX_STANDARD 20)
endif()

add_test(NAME MyTest COMMAND my_test)