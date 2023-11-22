#include <test_utils.h>

void test_string() {
    char tab1[] = "Salut !";
    char tab2[] = "Salut !";
    char tab3[] = "Salut!";
    assert_equals_arr_char(tab1, tab2, 8, "Should be equal strings!");
    assert_not_equals_arr_char(tab1, tab3, 8, "Should be not equals since salutations are different...");
}

void test_short_array() {
    short tab1[] = {1, 2, 3, 4};
    short tab2[] = {1, 2, 3, 4};
    short tab3[] = {1, 2, 4, 3};
    assert_equals_arr_short(tab1, tab2, 4, "Should be equal short arrays");
    assert_not_equals_arr_short(tab1, tab3, 4, "Should be not equal short arrays");
}

void test_int_array() {
    int tab1[] = {1, 2, 3, 4};
    int tab2[] = {1, 2, 3, 4};
    int tab3[] = {1, 2, 4, 3};
    assert_equals_arr_int(tab1, tab2, 4, "Should be equal int arrays");
    assert_not_equals_arr_int(tab1, tab3, 4, "Should be not equal int arrays");
}

void test_long_array() {
    long long tab1[] = {1, 2, 3, 4};
    long long tab2[] = {1, 2, 3, 4};
    long long tab3[] = {1, 2, 4, 3};
    assert_equals_arr_long(tab1, tab2, 4, "Should be equal long arrays");
    assert_not_equals_arr_long(tab1, tab3, 4, "Should be not equal long arrays");
}

int main() {
    test_string();
    test_short_array();
    test_int_array();
    test_long_array();
    return 0;
}