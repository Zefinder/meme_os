#include <test_utils.h>

void test_nullity() {
    int a[] = {1, 2};
    int* b = 0;

    assert_null(b, "Should be NULL");
    assert_not_null(a, "Should be not NULL");
}

int main() {
    test_nullity(); 
    return 0;
}