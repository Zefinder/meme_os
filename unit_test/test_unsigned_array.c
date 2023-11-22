#include <test_utils.h>

void test_uint8_array() {
    uint8_t tab1[] = {1, 2, 3, 4};
    uint8_t tab2[] = {1, 2, 3, 4};
    uint8_t tab3[] = {1, 3, 2, 4};
    assert_equals_arr_uint8(tab1, tab2, 4, "Should be equal uint8 arrays");
    assert_not_equals_arr_uint8(tab1, tab3, 4, "Should be not equal uint8 arrays");
}

void test_uint16_array() {
    uint16_t tab1[] = {1, 2, 3, 4};
    uint16_t tab2[] = {1, 2, 3, 4};
    uint16_t tab3[] = {1, 3, 2, 4};
    assert_equals_arr_uint16(tab1, tab2, 4, "Should be equal uint16 arrays");
    assert_not_equals_arr_uint16(tab1, tab3, 4, "Should be not equal uint16 arrays");
}

void test_uint32_array() {
    uint32_t tab1[] = {1, 2, 3, 4};
    uint32_t tab2[] = {1, 2, 3, 4};
    uint32_t tab3[] = {1, 3, 2, 4};
    assert_equals_arr_uint32(tab1, tab2, 4, "Should be equal uint32 arrays");
    assert_not_equals_arr_uint32(tab1, tab3, 4, "Should be not equal uint32 arrays");
}

void test_uint64_array() {
    uint64_t tab1[] = {1, 2, 3, 4};
    uint64_t tab2[] = {1, 2, 3, 4};
    uint64_t tab3[] = {1, 3, 2, 4};
    assert_equals_arr_uint64(tab1, tab2, 4, "Should be equal uint64 arrays");
    assert_not_equals_arr_uint64(tab1, tab3, 4, "Should be not equal uint64 arrays");
}

int main() {
    test_uint8_array();
    test_uint16_array();
    test_uint32_array();
    test_uint64_array();
    return 0;
}
