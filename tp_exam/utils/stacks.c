#include "stacks.h"
#include "../tasks/task_manager.h"

// Task index stack
int task_stack[TASK_NUMBER];
int task_stack_index;

void init_task_stack() {
    // We fill from 0 to TASK_NUMBER - 1
    for (int index = 0; index < TASK_NUMBER; index++) {
        task_stack[index] = index;
    }

    // and we put the index at the start
    task_stack_index = 0;
}

// Pushes the task index into the stack
void push_task_index(int value) {
    // If task_stack_index is already at the bottom, we do nothing
    if (task_stack_index != 0) {
        // We decrement and put the value at the new index
        task_stack[--task_stack_index] = value;
    }
}

// Pops the task index that is ready to use, removing it from the stack. If none returns -1
int pop_task_index() {
    // If no value is pointed (after TASK_NUMBER - 1), we return -1
    if (task_stack_index == TASK_NUMBER) {
        return -1;
    }

    // Else we return the task index value and we increase the task_stack_index
    int result = task_stack[task_stack_index++];
    return result;
}