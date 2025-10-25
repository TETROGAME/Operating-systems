cmake_minimum_required(VERSION 3.30)
project(Lab_3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

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

add_library(Lab3_lib
        source/ThreadHandler.cpp
        source/Array.cpp
        source/Marker.cpp
)

target_include_directories(Lab3_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(Lab_3 main.cpp)
target_link_libraries(Lab_3 PRIVATE Lab3_lib)


file(GLOB TEST_SOURCES
        CONFIGURE_DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/*_test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_*.cpp
)

#Generating executables for each testfile found in tests/
foreach(test_src IN LISTS TEST_SOURCES)
    get_filename_component(test_name ${test_src} NAME_WE)

    add_executable(${test_name} ${test_src})

    target_link_libraries(${test_name}
            PRIVATE
            Lab3_lib
            gtest_main
    )

    target_include_directories(${test_name}
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    add_test(NAME ${test_name} COMMAND ${test_name})
endforeach()