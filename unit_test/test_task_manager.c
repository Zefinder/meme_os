#include <test_utils.h>
#include <stdio.h>
#include "../tp_exam/task_manager.h"

void test_task_creation() {
    uint16_t a = 2;
    a = create_task();
    printf("%d\n", a);
}

void test_more_10_tasks() {

}

void test_ask_new_page() {

}

void test_ask_third_page() {

}

void test_ask_shared_page() {

}

void test_ask_22_shared_pages() {

}

void test_task_end() {

}

int main() {
    test_task_creation();
    test_more_10_tasks();
    test_ask_new_page();
    test_ask_third_page();
    test_ask_shared_page();
    test_ask_22_shared_pages();
    test_task_end;    
    return 0;
}
