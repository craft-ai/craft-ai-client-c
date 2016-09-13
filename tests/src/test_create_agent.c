#include "unit_tests.h"

#include <stdio.h>

CRAFT_TEST(test1) {
    CRAFT_TEST_ASSERT(7 == 7);
    return 0;
}

CRAFT_TEST(test2) {
    CRAFT_TEST_ASSERT(84 != 85);
    return 0;
}

CRAFT_TEST_MAIN() {
    CRAFT_TEST_RUN(test1);
    CRAFT_TEST_RUN(test2);
    return 0;
}
