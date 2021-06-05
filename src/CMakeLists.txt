cmake_minimum_required(VERSION 3.17)
project(x-server-exe C)
set(CMAKE_C_STANDARD 11)

add_subdirectory(conf)
add_subdirectory(http)

add_executable(x-server main.c)

target_link_libraries(x-server http)
target_link_libraries(x-server conf)
target_link_libraries(x-server cjson)