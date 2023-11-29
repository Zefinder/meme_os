#include <test_utils.h>
#include "../tp_exam/utils/stacks.h"

void test_init_stack() {
    init_task_stack();
    int expected_task_stack[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int got_task_stack[10];
    for (int index = 0; index < 10; index++) {
        got_task_stack[index] = pop_task_index();
    }

    assert_equals_arr_int(expected_task_stack, got_task_stack, 10, "Initialisation for task index stack wrong...");
    assert_equals_int(-1, pop_task_index(), "Should not be more than 10 tasks...");
}

void test_push_stack() {
    // We initialise the stack
    init_task_stack();

    // We pop 2 values
    pop_task_index();
    pop_task_index();

    // We push the first value
    push_task_index(0);

    int expected_task_stack[] = {0, 2, 3, 4, 5, 6, 7, 8, 9};
    int got_task_stack[9];
    for (int index = 0; index < 9; index++) {
        got_task_stack[index] = pop_task_index();
    }

    assert_equals_arr_int(expected_task_stack, got_task_stack, 9, "Push went wrong at some point...");
}

void test_full_push_stack() {
    // We initialise the stack
    init_task_stack();

    // We pop all values
    for (int index = 0; index < 10; index++) {
        pop_task_index();
    }

    // We push upside down
    for (int value = 0; value < 10; value++) {
        push_task_index(value);
    }

    int expected_task_stack[] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    int got_task_stack[10];
    for (int index = 0; index < 10; index++) {
        got_task_stack[index] = pop_task_index();
    }

    assert_equals_arr_int(expected_task_stack, got_task_stack, 10, "All should be upside down...");
}

int main() {
    test_init_stack();
    test_push_stack();
    test_full_push_stack();

    return 0;
}