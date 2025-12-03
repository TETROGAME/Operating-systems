cmake_minimum_required(VERSION 3.16)
project(NamedPipesLab)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.27")
    set(CMAKE_POLICY_VERSION_MINIMUM 3.10)
endif()

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/release-1.7.0.zip
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
FetchContent_MakeAvailable(googletest)

enable_testing()

if(DEFINED googletest_SOURCE_DIR)
    include_directories(${googletest_SOURCE_DIR}/include)
endif()

include_directories(headers)

set(COMMON_SOURCES
        src/Employee.cpp
        src/Request.cpp
        src/FileHelper.cpp
)

add_executable(server src/server.cpp ${COMMON_SOURCES})
add_executable(client src/client.cpp ${COMMON_SOURCES})

enable_testing()

file(GLOB TEST_SOURCES "tests/*.cpp")

foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SOURCE} ${COMMON_SOURCES}
            tests/test_e2e.cpp
            tests/test_employee.cpp
            tests/test_file_operations.cpp)
    target_link_libraries(${TEST_NAME} gtest gtest_main)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endforeach()