add_library(craftai craftai.c craftai.h)

if(CMAKE_COMPILER_IS_GNUCC)
    set_property( TARGET craftai APPEND_STRING PROPERTY COMPILE_FLAGS "-Wall -Werror")
elseif(MSVC)
    set_property( TARGET craftai APPEND_STRING PROPERTY COMPILE_FLAGS "/W4 /WX" )
endif()

install(FILES craftai.h craftai.c DESTINATION ./src)