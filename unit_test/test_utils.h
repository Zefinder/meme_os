#ifndef __TEST_UTILS__
#define __TEST_UTILS__

#ifndef uint8_t
typedef unsigned char          uint8_t;
#endif

#ifndef uint16_t
typedef unsigned short         uint16_t;
#endif

#ifndef uint32_t
typedef unsigned int           uint32_t;
#endif

#ifndef uint64_t
typedef unsigned long long     uint64_t;
#endif

// Number equality
void assertEquals_uint8(uint8_t expected, uint8_t got, const char* message);
void assertEquals_uint16(uint16_t expected, uint16_t got, const char* message);
void assertEquals_uint32(uint32_t expected, uint32_t got, const char* message);
void assertEquals_uint64(uint64_t expected, uint64_t got, const char* message);
void assertEquals_char(char expected, char got, const char* message);
void assertEquals_short(short expected, short got, const char* message);
void assertEquals_int(int expected, int got, const char* message);
void assertEquals_long(long long expected, long long got, const char* message);

void assertNotEquals_uint8(uint8_t expected, uint8_t got, const char* message);
void assertNotEquals_uint16(uint16_t expected, uint16_t got, const char* message);
void assertNotEquals_uint32(uint32_t expected, uint32_t got, const char* message);
void assertNotEquals_uint64(uint64_t expected, uint64_t got, const char* message);
void assertNotEquals_char(char expected, char got, const char* message);
void assertNotEquals_short(short expected, short got, const char* message);
void assertNotEquals_int(int expected, int got, const char* message);
void assertNotEquals_long(long long expected, long long got, const char* message);

// Array equality
void assertEquals_arr_uint8(uint8_t expected[], uint8_t got[], uint32_t array_size, const char* message);
void assertEquals_arr_uint16(uint16_t expected[], uint16_t got[], uint32_t array_size, const char* message);
void assertEquals_arr_uint32(uint32_t expected[], uint32_t got[], uint32_t array_size, const char* message);
void assertEquals_arr_uint64(uint64_t expected[], uint64_t got[], uint32_t array_size, const char* message);
void assertEquals_arr_char(char expected[], char got[], uint32_t array_size, const char* message);
void assertEquals_arr_short(short expected[], short got[], uint32_t array_size, const char* message);
void assertEquals_arr_int(int expected[], int got[], uint32_t array_size, const char* message);
void assertEquals_arr_long(long long expected[], long long got[], uint32_t array_size, const char* message);

void assertNotEquals_arr_uint8(uint8_t expected[], uint8_t got[], uint32_t array_size, const char* message);
void assertNotEquals_arr_uint16(uint16_t expected[], uint16_t got[], uint32_t array_size, const char* message);
void assertNotEquals_arr_uint32(uint32_t expected[], uint32_t got[], uint32_t array_size, const char* message);
void assertNotEquals_arr_uint64(uint64_t expected[], uint64_t got[], uint32_t array_size, const char* message);
void assertNotEquals_arr_char(char expected[], char got[], uint32_t array_size, const char* message);
void assertNotEquals_arr_short(short expected[], short got[], uint32_t array_size, const char* message);
void assertNotEquals_arr_int(int expected[], int got[], uint32_t array_size, const char* message);
void assertNotEquals_arr_long(long expected[], long got[], uint32_t array_size, const char* message);

// // Null equality
// void assertNull(void* expected, char* message);
// void assertNotNull(void* expected, char* message);

#endif