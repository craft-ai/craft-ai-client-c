add_library(jsonsl jsonsl.c jsonsl.h)

if(CMAKE_COMPILER_IS_GNUCC)
    set_property( TARGET jsonsl APPEND_STRING PROPERTY COMPILE_FLAGS "-Wall -Werror -ansi")
elseif(MSVC)
    set_property( TARGET jsonsl APPEND_STRING PROPERTY COMPILE_FLAGS "/W4 /WX /Za" )
endif()

install(FILES jsonsl.h jsonsl.c DESTINATION ./src/vendor)