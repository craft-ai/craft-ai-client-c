function(add_craft_test name)
    add_executable(${name} ${name}.c unit_tests.h)

    # Using craft ai
    include_directories(../../src)
    target_link_libraries(${name} craftai)

    add_test(
        NAME ${name}
        COMMAND $<TARGET_FILE:${name}>
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endfunction(add_craft_test)

add_craft_test(test_client_init_teardown_config)
add_craft_test(test_create_client)
