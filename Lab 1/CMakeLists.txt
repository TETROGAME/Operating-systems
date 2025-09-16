cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("Lab 1")

# Add source to this project's executable.
add_executable(Main Main.cpp employee.cpp)
add_executable(Creator Creator.cpp employee.cpp)
add_executable(Reporter Reporter.cpp employee.cpp)


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_target_properties(Creator Reporter Main PROPERTIES CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.