cmake_minimum_required(VERSION 3.0.0)
project(cweb VERSION 0.1.0)

# Set the output folder where your program will be created
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/include)

# Set PATH in project
set(PROJECT_SRC ${PROJECT_SOURCE_DIR}/src)
set(LIBRARY_PATH ${PROJECT_SOURCE_DIR}/include)

# Include lib
add_library(
    server STATIC
    ${LIBRARY_PATH}/server/server.c
    ${LIBRARY_PATH}/server/server.h
)

add_library(
    fileHandler STATIC
    ${LIBRARY_PATH}/fileHandler/fileHandler.c
    ${LIBRARY_PATH}/fileHandler/fileHandler.h
)

# Source files
add_executable(main ${PROJECT_SRC}/main.c)

target_link_libraries(main PRIVATE server fileHandler)

# Set CPACK
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
