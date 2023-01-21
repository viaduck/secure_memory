# Copyright (c) 2015-2023 The ViaDuck Project
#
# This file is part of SecureMemory.
#
# SecureMemory is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# SecureMemory is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with SecureMemory.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.8)
project(secure_memory)

# add own modules
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)

# user settable settings
set(OPTION_SECURE_UNIQUE_PTR ON CACHE BOOL "Erase memory on unique ptr deletion")

# glob includes and sources
file(GLOB_RECURSE SECURE_MEMORY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE SECURE_MEMORY_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

# set up library target
add_library(secure_memory STATIC ${SECURE_MEMORY_SOURCES} ${SECURE_MEMORY_HEADERS})

# require and enable c++17 support, to support header compilation with c++17 we have to set an interface definition
target_compile_features(secure_memory INTERFACE cxx_std_17)

# compile options
target_compile_options(secure_memory PRIVATE -Wall -Wextra)
if (OPTION_SECURE_UNIQUE_PTR)
    target_compile_definitions(secure_memory PUBLIC SECURE_MEMORY_OPTION_SECURE_UNIQUE_PTR)
endif()

# includes
target_include_directories(secure_memory PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        )

if (WIN32)
    target_link_libraries(secure_memory ws2_32)
endif()

# add test subdir
add_subdirectory(test)



