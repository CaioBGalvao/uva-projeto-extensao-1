#ifndef TEST_UTILS_H
#define TEST_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSERT_EQ(expected, actual) do { \
    if ((expected) != (actual)) { \
        fprintf(stderr, "%s:%d: Assertion failed: %s != %s (%d != %d)\n", __FILE__, __LINE__, #expected, #actual, (int)(expected), (int)(actual)); \
        exit(1); \
    } \
} while(0)

#define ASSERT_STR_EQ(expected, actual) do { \
    if (strcmp((expected), (actual)) != 0) { \
        fprintf(stderr, "%s:%d: Assertion failed: %s != %s (\"%s\" != \"%s\")\n", __FILE__, __LINE__, #expected, #actual, (expected), (actual)); \
        exit(1); \
    } \
} while(0)

#endif
