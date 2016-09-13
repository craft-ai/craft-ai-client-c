cmake_minimum_required(VERSION 2.8)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")

project(craft-ai-client-c)

set(CRAFTAI_VERSION_MAJOR 0)
set(CRAFTAI_VERSION_MINOR 1)

add_subdirectory(src)

enable_testing()
add_subdirectory(tests/src)
