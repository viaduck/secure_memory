# Copyright (c) 2015-2018 The ViaDuck Project
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
    add_executable(SecureMemory_Test ${SECUREMEMORY_SOURCES} ${SECUREMEMORY_HEADERS} ${TEST_FILES_SOURCES} ${TEST_FILES_HEADERS})
    target_link_libraries(SecureMemory_Test ${GTEST_BOTH_LIBRARIES} SecureMemory)
    if (NOT ANDROID)
        target_link_libraries(SecureMemory_Test pthread)
    endif()
endif()
