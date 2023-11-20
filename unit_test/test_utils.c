#include "test_utils.h"
#include <stdio.h>
#include <stdlib.h>

#define ERROR_HEADER_FORMAT "[ERROR] %s... "

void exit_test(const char* message) {
    printf("%s\n", message);
    exit(1);
}

// Asserts equality for unsigned chars
void assertEquals_uint8(uint8_t expected, uint8_t got, const char* message) {
    assertEquals_uint64(expected, got, message);
}

// Asserts equality for unsigned shorts
void assertEquals_uint16(uint16_t expected, uint16_t got, const char* message) {
    assertEquals_uint64(expected, got, message);
}

// Asserts equality for unsigned ints
void assertEquals_uint32(uint32_t expected, uint32_t got, const char* message) {
    assertEquals_uint64(expected, got, message);
}

// Asserts equality for unsigned longs
void assertEquals_uint64(uint64_t expected, uint64_t got, const char* message) {
    if (expected != got) {
        printf(ERROR_HEADER_FORMAT, "Values not equal");
        printf("Expected <%lld> but got <%lld> : ", expected, got);
        exit_test(message);
    }
}

// Asserts equality for chars
void assertEquals_char(char expected, char got, const char* message) {
    assertEquals_long(expected, got, message);
}

// Asserts equality for shorts
void assertEquals_short(short expected, short got, const char* message) {
    assertEquals_long(expected, got, message);
}

// Asserts equality for ints
void assertEquals_int(int expected, int got, const char* message) {
    assertEquals_long(expected, got, message);
}

// Asserts equality for longs
void assertEquals_long(long long expected, long long got, const char* message) {
    if (expected != got) {
        printf(ERROR_HEADER_FORMAT, "Values not equal");
        printf("Expected <%lld> but got <%lld> : ", expected, got);
        exit_test(message);
    }
}

// Asserts non equality for unsigned chars
void assertNotEquals_uint8(uint8_t expected, uint8_t got, const char* message) {
    assertNotEquals_uint64(expected, got, message);
}

// Asserts non equality for unsigned shorts
void assertNotEquals_uint16(uint16_t expected, uint16_t got, const char* message) {
    assertNotEquals_uint64(expected, got, message);
}

// Asserts non equality for unsigned ints
void assertNotEquals_uint32(uint32_t expected, uint32_t got, const char* message) {
    assertNotEquals_uint64(expected, got, message);
}

// Asserts non equality for unsigned longs
void assertNotEquals_uint64(uint64_t expected, uint64_t got, const char* message) {
    if (expected == got) {
        printf(ERROR_HEADER_FORMAT, "Values equal");
        printf("Expected not <%lld> but got <%lld> : ", expected, got);
        exit_test(message);
    }
}

// Asserts non equality for chars
void assertNotEquals_char(char expected, char got, const char* message) {
    assertNotEquals_long(expected, got, message);
}

// Asserts non equality for shorts
void assertNotEquals_short(short expected, short got, const char* message) {
    assertNotEquals_long(expected, got, message);
}

// Asserts non equality for ints
void assertNotEquals_int(int expected, int got, const char* message) {
    assertNotEquals_long(expected, got, message);
}

// Asserts non equality for longs
void assertNotEquals_long(long long expected, long long got, const char* message) {
    if (expected == got) {
        printf(ERROR_HEADER_FORMAT, "Values equal");
        printf("Expected not <%lld> but got <%lld> : ", expected, got);
        exit_test(message);
    }
}

void assertEquals_arr_uint8(uint8_t expected[], uint8_t got[], uint32_t array_size, const char* message) {
    // If expected is NULL then error if got not NULL
    if (expected == NULL) {
        if (got != NULL) {
            printf(ERROR_HEADER_FORMAT, "Arrays not equal");
            printf("Expected NULL but got not NULL : ");
            exit_test(message);
        } else {
            return;
        }
    }

    // Here expected is not NULL
    if (got == NULL) {
        printf(ERROR_HEADER_FORMAT, "Arrays not equal");
        printf("Expected not NULL but got NULL : ");
        exit_test(message);
    }

    // If one different then error
    for (int index = 0; index < array_size; index++) {
        if (expected[index] != got[index]) {
            printf(ERROR_HEADER_FORMAT, "Arrays not equal");
            printf("Index %d expected value <%d> but got <%d> : ", index, expected[index], got[index]);
            exit_test(message);
        }
    }
}

void assertEquals_arr_uint16(uint16_t expected[], uint16_t got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got != NULL) {
            printf(ERROR_HEADER_FORMAT, "Arrays not equal");
            printf("Expected NULL but got not NULL : ");
            exit_test(message);
        } else {
            return;
        }
    }

    if (got == NULL) {
        printf(ERROR_HEADER_FORMAT, "Arrays not equal");
        printf("Expected not NULL but got NULL : ");
        exit_test(message);
    }

    for (int index = 0; index < array_size; index++) {
        if (expected[index] != got[index]) {
            printf(ERROR_HEADER_FORMAT, "Arrays not equal");
            printf("Index %d expected value <%d> but got <%d> : ", index, expected[index], got[index]);
            exit_test(message);
        }
    }
}

void assertEquals_arr_uint32(uint32_t expected[], uint32_t got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got != NULL) {
            printf(ERROR_HEADER_FORMAT, "Arrays not equal");
            printf("Expected NULL but got not NULL : ");
            exit_test(message);
        } else {
            return;
        }
    }

    if (got == NULL) {
        printf(ERROR_HEADER_FORMAT, "Arrays not equal");
        printf("Expected not NULL but got NULL : ");
        exit_test(message);
    }

    for (int index = 0; index < array_size; index++) {
        if (expected[index] != got[index]) {
            printf(ERROR_HEADER_FORMAT, "Arrays not equal");
            printf("Index %d expected value <%d> but got <%d> : ", index, expected[index], got[index]);
            exit_test(message);
        }
    }
}

void assertEquals_arr_uint64(uint64_t expected[], uint64_t got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got != NULL) {
            printf(ERROR_HEADER_FORMAT, "Arrays not equal");
            printf("Expected NULL but got not NULL : ");
            exit_test(message);
        } else {
            return;
        }
    }

    if (got == NULL) {
        printf(ERROR_HEADER_FORMAT, "Arrays not equal");
        printf("Expected not NULL but got NULL : ");
        exit_test(message);
    }

    for (int index = 0; index < array_size; index++) {
        if (expected[index] != got[index]) {
            printf(ERROR_HEADER_FORMAT, "Arrays not equal");
            printf("Index %d expected value <%lld> but got <%lld> : ", index, expected[index], got[index]);
            exit_test(message);
        }
    }
}

void assertEquals_arr_char(char expected[], char got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got != NULL) {
            printf(ERROR_HEADER_FORMAT, "Arrays not equal");
            printf("Expected NULL but got not NULL : ");
            exit_test(message);
        } else {
            return;
        }
    }

    if (got == NULL) {
        printf(ERROR_HEADER_FORMAT, "Arrays not equal");
        printf("Expected not NULL but got NULL : ");
        exit_test(message);
    }

    for (int index = 0; index < array_size; index++) {
        if (expected[index] != got[index]) {
            printf(ERROR_HEADER_FORMAT, "Arrays not equal");
            printf("Index %1$d expected value <%2$c> (%2$d) but got <%3$c> (%3$d) : ", index, expected[index], got[index]);
            exit_test(message);
        }
    }
}

void assertEquals_arr_short(short expected[], short got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got != NULL) {
            printf(ERROR_HEADER_FORMAT, "Arrays not equal");
            printf("Expected NULL but got not NULL : ");
            exit_test(message);
        } else {
            return;
        }
    }

    if (got == NULL) {
        printf(ERROR_HEADER_FORMAT, "Arrays not equal");
        printf("Expected not NULL but got NULL : ");
        exit_test(message);
    }

    for (int index = 0; index < array_size; index++) {
        if (expected[index] != got[index]) {
            printf(ERROR_HEADER_FORMAT, "Arrays not equal");
            printf("Index %d expected value <%d> but got <%d> : ", index, expected[index], got[index]);
            exit_test(message);
        }
    }
}

void assertEquals_arr_int(int expected[], int got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got != NULL) {
            printf(ERROR_HEADER_FORMAT, "Arrays not equal");
            printf("Expected NULL but got not NULL : ");
            exit_test(message);
        } else {
            return;
        }
    }

    if (got == NULL) {
        printf(ERROR_HEADER_FORMAT, "Arrays not equal");
        printf("Expected not NULL but got NULL : ");
        exit_test(message);
    }

    for (int index = 0; index < array_size; index++) {
        if (expected[index] != got[index]) {
            printf(ERROR_HEADER_FORMAT, "Arrays not equal");
            printf("Index %d expected value <%d> but got <%d> : ", index, expected[index], got[index]);
            exit_test(message);
        }
    }
}

void assertEquals_arr_long(long long expected[], long long got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got != NULL) {
            printf(ERROR_HEADER_FORMAT, "Arrays not equal");
            printf("Expected NULL but got not NULL : ");
            exit_test(message);
        } else {
            return;
        }
    }

    if (got == NULL) {
        printf(ERROR_HEADER_FORMAT, "Arrays not equal");
        printf("Expected not NULL but got NULL : ");
        exit_test(message);
    }

    for (int index = 0; index < array_size; index++) {
        if (expected[index] != got[index]) {
            printf(ERROR_HEADER_FORMAT, "Arrays not equal");
            printf("Index %d expected value <%lld> but got <%lld> : ", index, expected[index], got[index]);
            exit_test(message);
        }
    }
}

void assertNotEquals_arr_uint8(uint8_t expected[], uint8_t got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got == NULL) {
            printf(ERROR_HEADER_FORMAT, "Arrays equal");
            printf("Expected not NULL but got NULL : ");
            exit_test(message);
        } else {
            return;
        }
    }

    if (got != NULL) {
        int number_equal = 0;
        for (int index = 0; index < array_size; index++) {
            if (expected[index] == got[index]) {
                number_equal++;
            }
        }

        // If all elements equal then error
        if (number_equal == array_size) {
            printf(ERROR_HEADER_FORMAT, "Arrays equal");
            exit_test(message);
        }
    }
}

void assertNotEquals_arr_uint16(uint16_t expected[], uint16_t got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got == NULL) {
            printf(ERROR_HEADER_FORMAT, "Arrays equal");
            printf("Expected not NULL but got NULL : ");
            exit_test(message);
        } else {
            return;
        }
    }

    if (got != NULL) {
        int number_equal = 0;
        for (int index = 0; index < array_size; index++) {
            if (expected[index] == got[index]) {
                number_equal++;
            }
        }

        if (number_equal == array_size) {
            printf(ERROR_HEADER_FORMAT, "Arrays equal");
            exit_test(message);
        }
    }
}

void assertNotEquals_arr_uint32(uint32_t expected[], uint32_t got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got == NULL) {
            printf(ERROR_HEADER_FORMAT, "Arrays equal");
            printf("Expected not NULL but got NULL : ");
            exit_test(message);
        } else {
            return;
        }
    }

    if (got != NULL) {
        int number_equal = 0;
        for (int index = 0; index < array_size; index++) {
            if (expected[index] == got[index]) {
                number_equal++;
            }
        }

        if (number_equal == array_size) {
            printf(ERROR_HEADER_FORMAT, "Arrays equal");
            exit_test(message);
        }
    }
}

void assertNotEquals_arr_uint64(uint64_t expected[], uint64_t got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got == NULL) {
            printf(ERROR_HEADER_FORMAT, "Arrays equal");
            printf("Expected not NULL but got NULL : ");
            exit_test(message);
        } else {
            return;
        }
    }

    if (got != NULL) {
        int number_equal = 0;
        for (int index = 0; index < array_size; index++) {
            if (expected[index] == got[index]) {
                number_equal++;
            }
        }

        if (number_equal == array_size) {
            printf(ERROR_HEADER_FORMAT, "Arrays equal");
            exit_test(message);
        }
    }
}

void assertNotEquals_arr_char(char expected[], char got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got == NULL) {
            printf(ERROR_HEADER_FORMAT, "Arrays equal");
            printf("Expected not NULL but got NULL : ");
            exit_test(message);
        } else {
            return;
        }
    }

    if (got != NULL) {
        int number_equal = 0;
        for (int index = 0; index < array_size; index++) {
            if (expected[index] == got[index]) {
                number_equal++;
            }
        }

        if (number_equal == array_size) {
            printf(ERROR_HEADER_FORMAT, "Arrays equal");
            exit_test(message);
        }
    }
}

void assertNotEquals_arr_short(short expected[], short got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got == NULL) {
            printf(ERROR_HEADER_FORMAT, "Arrays equal");
            printf("Expected not NULL but got NULL : ");
            exit_test(message);
        } else {
            return;
        }
    }

    if (got != NULL) {
        int number_equal = 0;
        for (int index = 0; index < array_size; index++) {
            if (expected[index] == got[index]) {
                number_equal++;
            }
        }

        if (number_equal == array_size) {
            printf(ERROR_HEADER_FORMAT, "Arrays equal");
            exit_test(message);
        }
    }
}

void assertNotEquals_arr_int(int expected[], int got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got == NULL) {
            printf(ERROR_HEADER_FORMAT, "Arrays equal");
            printf("Expected not NULL but got NULL : ");
            exit_test(message);
        } else {
            return;
        }
    }

    if (got != NULL) {
        int number_equal = 0;
        for (int index = 0; index < array_size; index++) {
            if (expected[index] == got[index]) {
                number_equal++;
            }
        }

        if (number_equal == array_size) {
            printf(ERROR_HEADER_FORMAT, "Arrays equal");
            exit_test(message);
        }
    }
}

void assertNotEquals_arr_long(long expected[], long got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got == NULL) {
            printf(ERROR_HEADER_FORMAT, "Arrays equal");
            printf("Expected not NULL but got NULL : ");
            exit_test(message);
        } else {
            return;
        }
    }

    if (got != NULL) {
        int number_equal = 0;
        for (int index = 0; index < array_size; index++) {
            if (expected[index] == got[index]) {
                number_equal++;
            }
        }

        if (number_equal == array_size) {
            printf(ERROR_HEADER_FORMAT, "Arrays equal");
            exit_test(message);
        }
    }
}

// Null equality
void assertNull(void* expected, char* message) {
    if (expected != NULL) {
        printf(ERROR_HEADER_FORMAT, "Got not NULL");
        printf("Expected NULL but got not NULL : ");
        exit_test(message);
    }
}

void assertNotNull(void* expected, char* message) {
    if (expected == NULL) {
        printf(ERROR_HEADER_FORMAT, "Got NULL");
        printf("Expected not NULL but got NULL : ");
        exit_test(message);
    }
}