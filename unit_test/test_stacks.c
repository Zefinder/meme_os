#include <test_utils.h>
#include <extend/memory.h>
#include <extend/stacks.h>

void test_init_task_stack() {
    init_task_stack();
    int expected_task_stack[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int got_task_stack[10];
    for (int index = 0; index < 10; index++) {
        got_task_stack[index] = pop_task_index();
    }

    assert_equals_arr_int(expected_task_stack, got_task_stack, 10, "Initialisation for task index stack wrong...");
    assert_equals_int(-1, pop_task_index(), "Should not be more than 10 tasks...");
}

void test_init_ptb_stack() {
    init_ptb_stack();

    uint32_t expected_ptb_stack[NUM_PTB];
    for (int i = 0; i < NUM_PTB; i++) {
        expected_ptb_stack[i] = (uint32_t)(PTB_START + i*PTB_SIZE);
    }

    uint32_t got_ptb_stack[NUM_PTB];
    for (int index = 0; index < NUM_PTB; index++) {
        got_ptb_stack[index] = (uint32_t)pop_ptb();
    }

    assert_equals_arr_uint32(expected_ptb_stack, got_ptb_stack, NUM_PTB, "Initialisation for ptb index stack wrong...");
    assert_equals_uint32(NULL, (uint32_t)pop_ptb(), "Should not be more than NUM_PTB ptbs...");
}

void test_push_task_stack() {
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

void test_push_ptb_stack() {
    // We initialise the stack
    init_ptb_stack();

    // We pop 2 values
    pop_ptb();
    pop_ptb();

    // We push the first value
    push_ptb((pte32_t*)(PTB_START));

    uint32_t expected_ptb_stack[NUM_PTB - 1];
    expected_ptb_stack[0] = (uint32_t)(PTB_START);
    for (int i = 1; i < NUM_PTB; i++) {
        expected_ptb_stack[i] = (uint32_t)(PTB_START + (i + 1)*PTB_SIZE);
    }

    uint32_t got_ptb_stack[NUM_PTB - 1];
    for (int index = 0; index < NUM_PTB - 1; index++) {
        got_ptb_stack[index] = (uint32_t)pop_ptb();
    }

    assert_equals_arr_int(expected_ptb_stack, got_ptb_stack, NUM_PTB - 1, "Push went wrong at some point...");
}

void test_full_push_task_stack() {
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

void test_full_push_ptb_stack() {
    // We initialise the stack
    init_ptb_stack();

    // We pop all values
    for (int index = 0; index < NUM_PTB; index++) {
        pop_ptb();
    }

    // We push upside down
    for (int i = 0; i < NUM_PTB; i++) {
        push_ptb((pte32_t*)(PTB_START + i*PTB_SIZE));
    }

    uint32_t expected_ptb_stack[NUM_PTB];
    for (int i = 0; i < NUM_PTB; i++) {
        expected_ptb_stack[i] = (uint32_t)(PTB_START + (NUM_PTB - 1 - i)*PTB_SIZE);
    }

    uint32_t got_ptb_stack[NUM_PTB];
    for (int index = 0; index < NUM_PTB; index++) {
        got_ptb_stack[index] = (uint32_t)pop_ptb();
    }

    assert_equals_arr_int(expected_ptb_stack, got_ptb_stack, 10, "All should be upside down...");
}

int main() {
    test_init_task_stack();
    test_push_task_stack();
    test_full_push_task_stack();
    
    test_init_ptb_stack();
    test_push_ptb_stack();
    test_full_push_ptb_stack();

    return 0;
}