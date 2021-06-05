set(http_source_file
    conn.c conn.h
    req.c req.h
    resp.c resp.h
)

add_library(http ${http_source_file})

target_link_libraries(http conf)