cmake_minimum_required(VERSION 3.17)
project(x-server C)
set(CMAKE_C_STANDARD 11)

# message(STATUS ${CMAKE_SOURCE_DIR})

# set source file directory
# aux_source_directory(${CMAKE_SOURCE_DIR}/src sourceList)

# add_executable(x-server ${sourceList})
add_subdirectory(src)
