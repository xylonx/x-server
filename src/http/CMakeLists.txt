# This file is part of the xylonx distribution (https://github.com/xylonx).
# Copyright (c) 2022 xylonx.
# 
# This program is free software: you can redistribute it and/or modify  
# it under the terms of the GNU General Public License as published by  
# the Free Software Foundation, version 3.
# 
# This program is distributed in the hope that it will be useful, but 
# WITHOUT ANY WARRANTY; without even the implied warranty of 
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
# General Public License for more details.
# 
# You should have received a copy of the GNU General Public License 
# along with this program. If not, see <http://www.gnu.org/licenses/>.
# 

set(
    handler_source
    handler/get.cc
    handler/post.cc
    handler/put.cc
    handler/delete.cc
)

add_library(handler ${handler_source})
target_link_libraries(handler spdlog)

set(
    http_source
    conn.cc
    request.cc
    server.cc
)

if(WITHOUT_MULTIPLEXING)
    set(
        http_source
        ${http_source}
        server_serially.cc
    )
else()
    if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        set(
            http_source
            ${http_source}
            server_epoll.cc
        )
    elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        set(
            http_source
            ${http_source}
            server_kqueue.cc
        )
    else()
        set(
            http_source
            ${http_source}
            server_select.cc
        )
    endif()
endif()

add_library(http ${http_source})

target_link_libraries(http handler)
target_link_libraries(http spdlog)