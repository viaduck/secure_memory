cmake_minimum_required(VERSION 3.1)
project(SecureMemory)

# user settable settings
set(OPTION_SECURE_UNIQUEPTR ON CACHE BOOL "Erase memory on unique ptr deletion")
if (OPTION_SECURE_UNIQUEPTR)
    add_definitions(-DOPTION_SECURE_UNIQUEPTR)
endif()

# enable and require c++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# glob includes and sources
file(GLOB_RECURSE SECUREMEMORY_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE SECUREMEMORY_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

# set up library target
add_library(SecureMemory STATIC ${SECUREMEMORY_SOURCES} ${SECUREMEMORY_HEADERS})

# compile options
target_compile_options(SecureMemory PRIVATE -Wall -Wextra)

# includes
target_include_directories(SecureMemory PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
        )

# add test subdir
add_subdirectory(test)



