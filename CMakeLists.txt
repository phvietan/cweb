cmake_minimum_required(VERSION 3.0.0)
project(cweb VERSION 0.1.0)

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/include)

include_directories(include)

# Set src directory
set(PROJECT_SRC ${PROJECT_SOURCE_DIR}/src)

# Src files
add_executable(cweb ${PROJECT_SRC}/server.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
