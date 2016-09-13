add_library(craftai craftai.c craftai.h)

# Using the system's curl library
find_package(CURL)
include_directories(${CURL_INCLUDE_DIRS})
target_link_libraries(craftai ${CURL_LIBRARIES})

# Compilation flags: ANSI C and errors
if(CMAKE_COMPILER_IS_GNUCC)
    set_property(TARGET craftai APPEND_STRING PROPERTY COMPILE_FLAGS "-Wall -Werror -ansi")
elseif(MSVC)
    set_property(TARGET craftai APPEND_STRING PROPERTY COMPILE_FLAGS "/W4 /WX /Za" )
endif()
