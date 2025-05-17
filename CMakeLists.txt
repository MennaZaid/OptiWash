cmake_minimum_required(VERSION 3.14)
project(OptiWash)

set(CMAKE_CXX_STANDARD 20)

# Use FetchContent to download GoogleTest once

# Clean problematic cache files if they exist
if(EXISTS "${CMAKE_BINARY_DIR}/_deps/googletest-subbuild/CMakeCache.txt")
    file(REMOVE "${CMAKE_BINARY_DIR}/_deps/googletest-subbuild/CMakeCache.txt")
    message(STATUS "Removed problematic CMakeCache.txt file")
endif()

set(CMAKE_CXX_STANDARD 20)

# Include Google Test
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/v1.14.0.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add your library
add_library(ItemLib
        Item.cpp Item.h
        Greedy.cpp Greedy.h
        Backtracking.cpp Backtracking.h
        LaundryProgramDatabase.cpp LaundryProgramDatabase.h
)

# Main executable
add_executable(OptiWash main.cpp)
target_link_libraries(OptiWash PRIVATE ItemLib)

# Tests executable
add_executable(run_tests tests.cpp)
target_link_libraries(run_tests
        PRIVATE
        ItemLib
        gtest
        gtest_main
)

# Enable testing
enable_testing()
add_test(NAME all_tests COMMAND run_tests)

# Platform specific pthread for Unix/Linux
if(UNIX AND NOT APPLE)
    target_link_libraries(run_tests PRIVATE pthread)
endif()
