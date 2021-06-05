set(conf_source_file conf.c conf.h)

message(STATUS ${CMAKE_SOURCE_DIR})

add_library(conf ${conf_source_file})