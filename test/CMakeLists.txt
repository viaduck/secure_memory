# find system gtest
find_package(GTest)

if (GTEST_FOUND)
    # glob test files
    file(GLOB_RECURSE TEST_FILES_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
    file(GLOB_RECURSE TEST_FILES_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

    # include relevant dirs
    include_directories(${CMAKE_CURRENT_SOURCE_DIR})
    include_directories(${GTEST_INCLUDE_DIRS})

    # set up test target
    add_executable(SecureMemory_Test ${SECUREMEMORY_SOURCES} ${SECUREMEMORY_HEADERS} ${TEST_FILES_SOURCES} ${TEST_FILES_HEADERS})
    target_link_libraries(SecureMemory_Test ${GTEST_BOTH_LIBRARIES} pthread SecureMemory)

endif()