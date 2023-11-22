#include <test_utils.h>

void test_uint8() {
    assert_equals_uint8(61, 61, "Same number...");
    assert_not_equals_uint8(62, 61, "Different number...");
}

void test_uint16() {
    assert_equals_uint16(2, 1+1, "Addition problem...");
    assert_not_equals_uint16(3, 1+1, "Get good loser!");
}

void test_uint32() {
    assert_equals_uint32(2, 1+1, "Addition problem...");
    assert_not_equals_uint32(3, 1+1, "Get good loser!");
}

void test_uint64() {
    assert_equals_uint64(2, 1+1, "Addition problem...");
    assert_not_equals_uint64(3, 1+1, "Get good loser!");
}

int main() {
    test_uint8();
    test_uint16();
    test_uint32();
    test_uint64();
    return 0;
}