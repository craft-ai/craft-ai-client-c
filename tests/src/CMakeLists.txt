function(add_craft_test name)
  add_executable(${name} ${name}.c unit_tests.h)
  if(CMAKE_COMPILER_IS_GNUCC)
      set_property(TARGET ${name} APPEND_STRING PROPERTY COMPILE_FLAGS "-Wall -Werror -ansi")
  elseif(MSVC)
      set_property(TARGET ${name} APPEND_STRING PROPERTY COMPILE_FLAGS "/W4 /WX /Za" )
  endif()
  add_test(
      NAME ${name}
      COMMAND $<TARGET_FILE:${name}>)
endfunction(add_craft_test)

add_craft_test(test_create_agent)
