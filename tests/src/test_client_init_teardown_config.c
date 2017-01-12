#include "unit_tests.h"

#include "craftai.h"

#include <stdio.h>

CRAFT_TEST(init_teardown) {
    CRAFT_TEST_ASSERT_CALL_OK(craft_init());
    CRAFT_TEST_ASSERT_CALL_OK(craft_teardown());
    return 0;
}

CRAFT_TEST(init_twice) {
    CRAFT_TEST_ASSERT_CALL_OK(craft_init());
    CRAFT_TEST_ASSERT(craft_init() == CRAFTAI_ALREADY_INITIALIZED);
    return 0;
}

CRAFT_TEST(config_token) {
    CRAFT_TEST_ASSERT_CALL_OK(craft_config(CRAFTAI_TOKEN, "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiYWRtaW4iOnRydWV9.TJVA95OrM7E2cBab30RMHrHDcEfxjoYZgeFONFh7HgQ"));
    return 0;
}

CRAFT_TEST(config_owner) {
    CRAFT_TEST_ASSERT_CALL_OK(craft_config(CRAFTAI_OWNER, "gisele"));
    return 0;
}

CRAFT_TEST(config_url) {
    CRAFT_TEST_ASSERT_CALL_OK(craft_config(CRAFTAI_URL, "https://foo.example.com"));
    return 0;
}

CRAFT_TEST(config_unknown) {
    CRAFT_TEST_ASSERT(craft_config(756, "hey") == CRAFTAI_UNKNOWN_CONFIG_OPTION);
    return 0;
}

CRAFT_TEST_MAIN() {
    CRAFT_TEST_RUN(init_teardown);
    CRAFT_TEST_RUN(init_twice);
    CRAFT_TEST_RUN(config_token);
    CRAFT_TEST_RUN(config_owner);
    CRAFT_TEST_RUN(config_url);
    CRAFT_TEST_RUN(config_unknown);
    return 0;
}
