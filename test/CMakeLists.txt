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

include(FindOrBuildGTest)

if (GTEST_FOUND)
    # glob test files
    file(GLOB_RECURSE TEST_FILES_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
    file(GLOB_RECURSE TEST_FILES_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

    # include relevant dirs
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    include_directories(${GTEST_INCLUDE_DIRS})

    # set up test target
    add_executable(secure_memory_test ${SECURE_MEMORY_SOURCES} ${SECURE_MEMORY_HEADERS} ${TEST_FILES_SOURCES} ${TEST_FILES_HEADERS})

    # do not shred when using tests since they may allocate huge amounts of memory (affects this directory and below)
    remove_definitions(-DSECURE_MEMORY_OPTION_SECURE_UNIQUE_PTR)

    # enable additional warnings
    target_compile_options(secure_memory_test PRIVATE -Wall -Wextra)

    # add sanitization compile options
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND NOT ANDROID AND ENABLE_SAN)
        message(STATUS "Sanitization compile options enabled.")
        target_compile_options(secure_memory_test PRIVATE -ftrapv -fsanitize=integer)
        target_link_options(secure_memory_test PRIVATE -fsanitize=integer)
    else()
        message(STATUS "Sanitization compile options NOT enabled.")
    endif()
    target_link_libraries(secure_memory_test ${GTEST_BOTH_LIBRARIES} secure_memory)
    if (NOT ANDROID)
        target_link_libraries(secure_memory_test pthread)
    endif()
endif()
