#include "test_utils.h"
#include <stdlib.h>
#include <stdio.h>

#define ERROR_HEADER_FORMAT "\033[0;31m[ERROR] %s... "

#define printf_error_header(s)                                  printf("\033[0;31m[ERROR] in %s: %s... \033[0m", __func__, s)
#define printf_error_expected_got(f,exp,got)                    printf("\033[0;31m" f "\033[0m", exp, got)
#define printf_error_arr_expected_got(f,index,exp,got)          printf("\033[0;31m" f "\033[0m", index, exp, got)
#define printf_error_message(s)                                 printf("\033[0;31m%s\033[0m\n", s)
#define printf_error(header,format,expected,got,message)        \
    ({                                                          \
        printf_error_header(header);                            \
        printf_error_expected_got(format, expected, got);       \
        printf_error_message(message);                          \
        exit(1);                                                \
    })
#define printf_array_error(header,format,index,expected,got,message)    \
    ({                                                                  \
        printf_error_header(header);                                    \
        printf_error_arr_expected_got(format, index, expected, got);    \
        printf_error_message(message);                                  \
        exit(1);                                                        \
    })

// Asserts equality for unsigned chars
void assert_equals_uint8(uint8_t expected, uint8_t got, const char* message) {
    if (expected != got) {
        printf_error("Values not equal", "Expected <%c> but got <%c> : ", expected, got, message);
    }
}

// Asserts equality for unsigned shorts
void assert_equals_uint16(uint16_t expected, uint16_t got, const char* message) {
    if (expected != got) {
        printf_error("Values not equal", "Expected <%d> but got <%d> : ", expected, got, message);
    }
}

// Asserts equality for unsigned ints
void assert_equals_uint32(uint32_t expected, uint32_t got, const char* message) {
    if (expected != got) {
        printf_error("Values not equal", "Expected <%d> but got <%d> : ", expected, got, message);
    }
}

// Asserts equality for unsigned longs
void assert_equals_uint64(uint64_t expected, uint64_t got, const char* message) {
    if (expected != got) {
        printf_error("Values not equal", "Expected <%lld> but got <%lld> : ", expected, got, message);
    }
}

// Asserts equality for chars
void assert_equals_char(char expected, char got, const char* message) {
    if (expected != got) {
        printf_error("Values not equal", "Expected <%c> but got <%c> : ", expected, got, message);
    }
}

// Asserts equality for shorts
void assert_equals_short(short expected, short got, const char* message) {
    if (expected != got) {
        printf_error("Values not equal", "Expected <%d> but got <%d> : ", expected, got, message);
    }
}

// Asserts equality for ints
void assert_equals_int(int expected, int got, const char* message) {
    if (expected != got) {
        printf_error("Values not equal", "Expected <%d> but got <%d> : ", expected, got, message);
    }
}

// Asserts equality for longs
void assert_equals_long(long long expected, long long got, const char* message) {
    if (expected != got) {
        printf_error("Values not equal", "Expected <%lld> but got <%lld> : ", expected, got, message);
    }
}

// Asserts non equality for unsigned chars
void assert_not_equals_uint8(uint8_t expected, uint8_t got, const char* message) {
    if (expected == got) {
        printf_error("Values equal", "Expected not <%c> but got <%c> : ", expected, got, message);
    }
}

// Asserts non equality for unsigned shorts
void assert_not_equals_uint16(uint16_t expected, uint16_t got, const char* message) {
    if (expected == got) {
        printf_error("Values equal", "Expected not <%d> but got <%d> : ", expected, got, message);
    }
}

// Asserts non equality for unsigned ints
void assert_not_equals_uint32(uint32_t expected, uint32_t got, const char* message) {
    if (expected == got) {
        printf_error("Values equal", "Expected not <%d> but got <%d> : ", expected, got, message);
    }
}

// Asserts non equality for unsigned longs
void assert_not_equals_uint64(uint64_t expected, uint64_t got, const char* message) {
    if (expected == got) {
        printf_error("Values equal", "Expected not <%lld> but got <%lld> : ", expected, got, message);
    }
}

// Asserts non equality for chars
void assert_not_equals_char(char expected, char got, const char* message) {
    if (expected == got) {
        printf_error("Values equal", "Expected not <%c> but got <%c> : ", expected, got, message);
    }
}

// Asserts non equality for shorts
void assert_not_equals_short(short expected, short got, const char* message) {
    if (expected == got) {
        printf_error("Values equal", "Expected not <%d> but got <%d> : ", expected, got, message);
    }
}

// Asserts non equality for ints
void assert_not_equals_int(int expected, int got, const char* message) {
    if (expected == got) {
        printf_error("Values equal", "Expected not <%d> but got <%d> : ", expected, got, message);
    }
}

// Asserts non equality for longs
void assert_not_equals_long(long long expected, long long got, const char* message) {
    if (expected == got) {
        printf_error("Values equal", "Expected not <%lld> but got <%lld> : ", expected, got, message);
    }
}

void assert_equals_arr_uint8(uint8_t expected[], uint8_t got[], uint32_t array_size, const char* message) {
    // If expected is NULL then error if got not NULL
    if (expected == NULL) {
        if (got != NULL) {
            printf_error("Arrays not equal", "Expected %s but got not %s : ", "NULL", "not NULL", message);
        } else {
            return;
        }
    }

    // Here expected is not NULL
    if (got == NULL) {
        printf_error("Arrays not equal", "Expected %s but got %s : ", "not NULL", "NULL", message);
    }

    // If one different then error
    for (int index = 0; index < array_size; index++) {
        if (expected[index] != got[index]) {
            printf_array_error("Arrays not equal", "Index %d expected value <%d> but got <%d> : ", index,
                                expected[index], got[index], message);
        }
    }
}

void assert_equals_arr_uint16(uint16_t expected[], uint16_t got[], uint32_t array_size, const char* message) {
    // If expected is NULL then error if got not NULL
    if (expected == NULL) {
        if (got != NULL) {
            printf_error("Arrays not equal", "Expected %s but got not %s : ", "NULL", "not NULL", message);
        } else {
            return;
        }
    }

    // Here expected is not NULL
    if (got == NULL) {
        printf_error("Arrays not equal", "Expected %s but got %s : ", "not NULL", "NULL", message);
    }

    // If one different then error
    for (int index = 0; index < array_size; index++) {
        if (expected[index] != got[index]) {
            printf_array_error("Arrays not equal", "Index %d expected value <%d> but got <%d> : ", index,
                                expected[index], got[index], message);
        }
    }
}

void assert_equals_arr_uint32(uint32_t expected[], uint32_t got[], uint32_t array_size, const char* message) {
    // If expected is NULL then error if got not NULL
    if (expected == NULL) {
        if (got != NULL) {
            printf_error("Arrays not equal", "Expected %s but got not %s : ", "NULL", "not NULL", message);
        } else {
            return;
        }
    }

    // Here expected is not NULL
    if (got == NULL) {
        printf_error("Arrays not equal", "Expected %s but got %s : ", "not NULL", "NULL", message);
    }

    // If one different then error
    for (int index = 0; index < array_size; index++) {
        if (expected[index] != got[index]) {
            printf_array_error("Arrays not equal", "Index %d expected value <%d> but got <%d> : ", index,
                                expected[index], got[index], message);
        }
    }
}

void assert_equals_arr_uint64(uint64_t expected[], uint64_t got[], uint32_t array_size, const char* message) {
    // If expected is NULL then error if got not NULL
    if (expected == NULL) {
        if (got != NULL) {
            printf_error("Arrays not equal", "Expected %s but got not %s : ", "NULL", "not NULL", message);
        } else {
            return;
        }
    }

    // Here expected is not NULL
    if (got == NULL) {
        printf_error("Arrays not equal", "Expected %s but got %s : ", "not NULL", "NULL", message);
    }

    // If one different then error
    for (int index = 0; index < array_size; index++) {
        if (expected[index] != got[index]) {
            printf_array_error("Arrays not equal", "Index %d expected value <%lld> but got <%lld> : ", index,
                                expected[index], got[index], message);
        }
    }
}

void assert_equals_arr_char(char expected[], char got[], uint32_t array_size, const char* message) {
    // If expected is NULL then error if got not NULL
    if (expected == NULL) {
        if (got != NULL) {
            printf_error("Arrays not equal", "Expected %s but got not %s : ", "NULL", "not NULL", message);
        } else {
            return;
        }
    }

    // Here expected is not NULL
    if (got == NULL) {
        printf_error("Arrays not equal", "Expected %s but got %s : ", "not NULL", "NULL", message);
    }

    // If one different then error
    for (int index = 0; index < array_size; index++) {
        if (expected[index] != got[index]) {
            printf_array_error("Arrays not equal", "Index %d expected value <%d> but got <%d> : ", index,
                                expected[index], got[index], message);
        }
    }
}

void assert_equals_arr_short(short expected[], short got[], uint32_t array_size, const char* message) {
    // If expected is NULL then error if got not NULL
    if (expected == NULL) {
        if (got != NULL) {
            printf_error("Arrays not equal", "Expected %s but got not %s : ", "NULL", "not NULL", message);
        } else {
            return;
        }
    }

    // Here expected is not NULL
    if (got == NULL) {
        printf_error("Arrays not equal", "Expected %s but got %s : ", "not NULL", "NULL", message);
    }

    // If one different then error
    for (int index = 0; index < array_size; index++) {
        if (expected[index] != got[index]) {
            printf_array_error("Arrays not equal", "Index %d expected value <%d> but got <%d> : ", index,
                                expected[index], got[index], message);
        }
    }
}

void assert_equals_arr_int(int expected[], int got[], uint32_t array_size, const char* message) {
    // If expected is NULL then error if got not NULL
    if (expected == NULL) {
        if (got != NULL) {
            printf_error("Arrays not equal", "Expected %s but got not %s : ", "NULL", "not NULL", message);
        } else {
            return;
        }
    }

    // Here expected is not NULL
    if (got == NULL) {
        printf_error("Arrays not equal", "Expected %s but got %s : ", "not NULL", "NULL", message);
    }

    // If one different then error
    for (int index = 0; index < array_size; index++) {
        if (expected[index] != got[index]) {
            printf_array_error("Arrays not equal", "Index %d expected value <%d> but got <%d> : ", index,
                                expected[index], got[index], message);
        }
    }
}

void assert_equals_arr_long(long long expected[], long long got[], uint32_t array_size, const char* message) {
    // If expected is NULL then error if got not NULL
    if (expected == NULL) {
        if (got != NULL) {
            printf_error("Arrays not equal", "Expected %s but got not %s : ", "NULL", "not NULL", message);
        } else {
            return;
        }
    }

    // Here expected is not NULL
    if (got == NULL) {
        printf_error("Arrays not equal", "Expected %s but got %s : ", "not NULL", "NULL", message);
    }

    // If one different then error
    for (int index = 0; index < array_size; index++) {
        if (expected[index] != got[index]) {
            printf_array_error("Arrays not equal", "Index %d expected value <%lld> but got <%lld> : ", index,
                                expected[index], got[index], message);
        }
    }
}

void assert_not_equals_arr_uint8(uint8_t expected[], uint8_t got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got == NULL) {
            printf_error("Arrays equal", "Expected %s but got %s : ", "not NULL", "NULL", message);
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
            printf_error("Arrays equal", "Expected %s but got %s : ", "not equal", "equal", message);
        }
    }
}

void assert_not_equals_arr_uint16(uint16_t expected[], uint16_t got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got == NULL) {
            printf_error("Arrays equal", "Expected %s but got %s : ", "not NULL", "NULL", message);
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
            printf_error("Arrays equal", "Expected %s but got %s : ", "not equal", "equal", message);
        }
    }
}

void assert_not_equals_arr_uint32(uint32_t expected[], uint32_t got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got == NULL) {
            printf_error("Arrays equal", "Expected %s but got %s : ", "not NULL", "NULL", message);
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
            printf_error("Arrays equal", "Expected %s but got %s : ", "not equal", "equal", message);
        }
    }
}

void assert_not_equals_arr_uint64(uint64_t expected[], uint64_t got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got == NULL) {
            printf_error("Arrays equal", "Expected %s but got %s : ", "not NULL", "NULL", message);
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
            printf_error("Arrays equal", "Expected %s but got %s : ", "not equal", "equal", message);
        }
    }
}

void assert_not_equals_arr_char(char expected[], char got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got == NULL) {
            printf_error("Arrays equal", "Expected %s but got %s : ", "not NULL", "NULL", message);
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
            printf_error("Arrays equal", "Expected %s but got %s : ", "not equal", "equal", message);
        }
    }
}

void assert_not_equals_arr_short(short expected[], short got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got == NULL) {
            printf_error("Arrays equal", "Expected %s but got %s : ", "not NULL", "NULL", message);
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
            printf_error("Arrays equal", "Expected %s but got %s : ", "not equal", "equal", message);
        }
    }
}

void assert_not_equals_arr_int(int expected[], int got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got == NULL) {
            printf_error("Arrays equal", "Expected %s but got %s : ", "not NULL", "NULL", message);
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
            printf_error("Arrays equal", "Expected %s but got %s : ", "not equal", "equal", message);
        }
    }
}

void assert_not_equals_arr_long(long long expected[], long long got[], uint32_t array_size, const char* message) {
    if (expected == NULL) {
        if (got == NULL) {
            printf_error("Arrays equal", "Expected %s but got %s : ", "not NULL", "NULL", message);
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
            printf_error("Arrays equal", "Expected %s but got %s : ", "not equal", "equal", message);
        }
    }
}

// Null equality
void assert_null(void* expected, char* message) {
    if (expected != NULL) {
        printf_error("Got not NULL", "Expected %s but got %s : ", "NULL", "not NULL", message);
    }
}

void assert_not_null(void* expected, char* message) {
    if (expected == NULL) {
        printf_error("Got NULL", "Expected %s but got %s : ", "not NULL", "NULL", message);
    }
}
