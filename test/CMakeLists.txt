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
    # collect test files
    file(GLOB_RECURSE TEST_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

    macro(make_viaduck_test_target name)
        add_executable(${name} ${SECURE_MEMORY_FILES} ${TEST_FILES})

        # includes
        target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
        # link deps
        target_link_libraries(${name} PUBLIC secure_memory ${GTEST_TARGET})
        # enable additional warnings
        target_compile_options(${name} PRIVATE -Wall -Wextra)
        # do not shred when using tests since they may allocate huge amounts of memory (affects this directory and below)
        remove_definitions(-DSECURE_MEMORY_OPTION_SECURE_UNIQUE_PTR)
    endmacro()
    make_viaduck_test_target(secure_memory_test)

    include(EnableSanitizers)
    enable_sanitizers_for_target(secure_memory_test)

    include(EnableCoverage)
    enable_coverage_for_target(secure_memory_test ${CMAKE_CURRENT_SOURCE_DIR}/../
        "test/*" "${GTEST_SRC_DIR}/*")
endif()
