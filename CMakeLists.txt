# Copyright (c) 2015-2024 The ViaDuck Project
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

cmake_minimum_required(VERSION 3.18)
project(secure_memory)

# user settable settings
option(SECURE_MEMORY_UNIQUE_PTR_SHRED "Erase memory on unique ptr deletion" ON)
option(SECURE_MEMORY_BUILD_TESTS "Enable test compilation for secure memory" OFF)

# add own modules
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)

# collect files
file(GLOB_RECURSE SECURE_MEMORY_FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

# set up library target
add_library(secure_memory STATIC ${SECURE_MEMORY_FILES})
# includes
target_include_directories(secure_memory PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
# link deps
if (WIN32)
    target_link_libraries(secure_memory ws2_32)
endif()
# require and enable c++17 support
target_compile_features(secure_memory PUBLIC cxx_std_17)
# compile options
target_compile_options(secure_memory PRIVATE -Wall -Wextra)
if (SECURE_MEMORY_UNIQUE_PTR_SHRED)
    target_compile_definitions(secure_memory PUBLIC SECURE_MEMORY_UNIQUE_PTR_SHRED)
endif()

# add test subdir
if (SECURE_MEMORY_BUILD_TESTS)
    add_subdirectory(test)
endif()
