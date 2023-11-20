#include "test_utils.h"

void test_addition() {
    assertEquals_int(2, 1+1, "Addition problem...");
    assertNotEquals_int(3, 1+1, "Get good loser!");
}

void test_soustraction() {
    assertEquals_int(0, 1-1, "Subtraction problem...");
    assertNotEquals_int(3, 1-1, "Get good loser!");
}

void test_multiplication() {
    assertEquals_int(0, 400*0, "Multiplication problem...");
    assertNotEquals_int(3, 3*2, "Get good loser!");
}

void test_uint8_tab() {
    uint8_t tab1[] = {1, 2, 3, 4};
    uint8_t tab2[] = {1, 2, 3, 4};
    uint8_t tab3[] = {1, 3, 2, 4};
    assertEquals_arr_uint8(tab1, tab2, 4, "Should be equal tabs!");
    assertNotEquals_arr_uint8(tab1, tab3, 4, "Should be not equals since 2 and 3 are inverted...");
}

void test_string() {
    char tab1[] = "Salut !";
    char tab2[] = "Salut !";
    char tab3[] = "Salut!";
    assertEquals_arr_char(tab1, tab2, 8, "Should be equal strings!");
    assertNotEquals_arr_char(tab1, tab3, 8, "Should be not equals since salutations are different...");
}

void test_nullity() {
    int a[] = {1, 2};
    int* b = 0;

    assertNull(b, "Should be NULL");
    assertNotNull(a, "Should be not NULL");
}

int main() {
    test_addition();
    test_soustraction();
    test_multiplication();
    test_uint8_tab();
    test_string();
    test_nullity(); 
    return 0;
}