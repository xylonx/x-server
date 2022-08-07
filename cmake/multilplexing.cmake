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

if(WITHOUT_MULTIPLEXING)
    message("WITHOUT_MULTIPLEXING defined. server will handle requests serially")
else()
    if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
        message("Current OS is Linux. USE epoll")
        set(WITH_EPOLL 1)
    elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
        message("Current OS is Darwin. USE kqueue")
        SET(WITH_KQUEUE 1)
    else()
        message("USE select")
        SET(WITH_SELECT 1)
    endif()
endif()