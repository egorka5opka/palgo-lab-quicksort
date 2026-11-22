cmake_minimum_required(VERSION 3.16)
project(lab_quicksort LANGUAGES CXX)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Use the existing local checkout; no dependency downloads are needed.
add_subdirectory(parlaylib EXCLUDE_FROM_ALL)
add_subdirectory(src)

include(CTest)
option(BUILD_BENCHMARKS "Build sorting benchmarks" ON)
if(BUILD_TESTING OR BUILD_BENCHMARKS)
    add_subdirectory(test)
endif()
