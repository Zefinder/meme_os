#include <test_utils.h>

void test_char() {
    assert_equals_char('a', 'a', "Same letter...");
    assert_not_equals_char('b', 'a', "Different letter...");
}

void test_short() {
    assert_equals_short(2, 1+1, "Addition problem...");
    assert_not_equals_short(3, 1+1, "Get good loser!");
}

void test_int() {
    assert_equals_int(2, 1+1, "Addition problem...");
    assert_not_equals_int(3, 1+1, "Get good loser!");
}

void test_long() {
    assert_equals_long(2, 1+1, "Addition problem...");
    assert_not_equals_long(3, 1+1, "Get good loser!");
}

int main() {
    test_char();
    test_short();
    test_int();
    test_long();
    return 0;
}
