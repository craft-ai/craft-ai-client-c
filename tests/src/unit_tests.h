#ifndef CRAFTAI_UNIT_TESTS_H
#define CRAFTAI_UNIT_TESTS_H

#include "dot_env.h"

#include <string.h>

#define STRINGIZE(x) STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING STRINGIZE(__LINE__)

/* Assertions */
#define CRAFT_TEST_ASSERT_MSG(test, message) \
do { \
    if (!(test)) { \
        return __FILE__ " (l" LINE_STRING "): " message; \
    } \
} while (0);

#define CRAFT_TEST_ASSERT(test) CRAFT_TEST_ASSERT_MSG(test, "Assertion \"" #test "\" failed!")

#define CRAFT_TEST_ASSERT_CALL_OK(craft_call) CRAFT_TEST_ASSERT_MSG(craft_call == CRAFTAI_OK, "\"" #craft_call "\" failed!")

#define CRAFT_TEST_ASSERT_STR_EQ(str, ref) CRAFT_TEST_ASSERT_MSG(strcmp(str, ref) == 0, "\"" #str "\" does not equal \"" #ref "\"!")

static int craftai_unit_tests_count = 0;

#define CRAFT_TEST(test) static char* test_ ## test ()

#define CRAFT_TEST_RUN(test) \
do { \
    char *message = test_ ## test (); \
    craftai_unit_tests_count++; \
    if (message) return message; \
} while (0);

#define CRAFT_TEST_MAIN() \
static char* craftai_run_tests(); \
int main(int argc, char **argv) { \
    dot_env_status_t s = dot_env_load(); \
    if (s == DOT_ENV_ERROR) { \
      return -1; \
    } \
    char *result = craftai_run_tests(); \
    if (result != 0) { \
        printf("%s\n", result); \
    } \
    return result != 0; \
} \
 \
static char* craftai_run_tests()

#endif /* CRAFTAI_UNIT_TESTS_H */
