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
    argparse_source
    argparse.cc
)

add_library(argparse ${argparse_source})

target_link_libraries(argparse spdlog)