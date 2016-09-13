#ifndef CRAFTAI_UNIT_TESTS_H
#define CRAFTAI_UNIT_TESTS_H

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
    char *result = craftai_run_tests(); \
    if (result != 0) { \
        printf("%s\n", result); \
    } \
    return result != 0; \
} \
 \
static char* craftai_run_tests()

#endif /* CRAFTAI_UNIT_TESTS_H */
