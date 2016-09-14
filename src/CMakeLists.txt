# Own sources
set(CRAFT_SOURCES craftai.c craftai.h)

# YAJL sources
get_filename_component(YAJL_SOURCES_DIR vendor/yajl/src/ ABSOLUTE)
set(YAJL_SOURCES
    ${YAJL_SOURCES_DIR}/yajl_alloc.c
    ${YAJL_SOURCES_DIR}/yajl_alloc.h
    ${YAJL_SOURCES_DIR}/yajl_buf.c
    ${YAJL_SOURCES_DIR}/yajl_buf.h
    ${YAJL_SOURCES_DIR}/yajl_encode.c
    ${YAJL_SOURCES_DIR}/yajl_encode.h
    ${YAJL_SOURCES_DIR}/yajl_gen.c
    ${YAJL_SOURCES_DIR}/yajl_lex.c
    ${YAJL_SOURCES_DIR}/yajl_lex.h
    ${YAJL_SOURCES_DIR}/yajl_parser.c
    ${YAJL_SOURCES_DIR}/yajl_parser.h
    ${YAJL_SOURCES_DIR}/yajl_tree.c
    ${YAJL_SOURCES_DIR}/yajl_version.c
    ${YAJL_SOURCES_DIR}/yajl.c
)
set(YAJL_PUBLIC_HEADERS
    ${YAJL_SOURCES_DIR}/api/yajl_common.h
    ${YAJL_SOURCES_DIR}/api/yajl_gen.h
    ${YAJL_SOURCES_DIR}/api/yajl_parse.h
    ${YAJL_SOURCES_DIR}/api/yajl_tree.h
)
include_directories(${YAJL_SOURCES_DIR})

# Copy api to have an API organized as YAJL expects
get_filename_component(INTERMEDIATE_OUT_DIR ../out/intermediate ABSOLUTE)
file(COPY ${YAJL_PUBLIC_HEADERS} DESTINATION ${INTERMEDIATE_OUT_DIR}/yajl)

# Configure the YAJL version file
set(YAJL_MAJOR 2)
set(YAJL_MINOR 1)
set(YAJL_MICRO 0)
set(YAJL_API_VERSION_FILE ${YAJL_SOURCES_DIR}/api/yajl_version.h.cmake)
set(dollar $)
configure_file(${YAJL_API_VERSION_FILE} ${INTERMEDIATE_OUT_DIR}/yajl/yajl_version.h)
include_directories(${INTERMEDIATE_OUT_DIR})

# Define the target
add_library(craftai ${CRAFT_SOURCES} ${YAJL_SOURCES})

# Using the system's curl library
find_package(CURL)
include_directories(${CURL_INCLUDE_DIRS})
target_link_libraries(craftai ${CURL_LIBRARIES})

# Compilation flags: ANSI C and errors
if ("${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
    # using Clang
    set_property(TARGET craftai APPEND_STRING PROPERTY COMPILE_FLAGS "-Wall -Werror -ansi -pedantic-errors")
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "GNU")
    # using GCC
    set_property(TARGET craftai APPEND_STRING PROPERTY COMPILE_FLAGS "-Wall -Werror -ansi -pedantic-errors")
elseif ("${CMAKE_C_COMPILER_ID}" MATCHES "MSVC")
    # using Visual Studio C++
    set_property(TARGET craftai APPEND_STRING PROPERTY COMPILE_FLAGS "/W4 /WX /Za" )
endif()
