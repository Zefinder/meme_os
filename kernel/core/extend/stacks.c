#include <extend/types.h>
#include <extend/stacks.h>
#include <extend/memory.h>
#include <extend/pagemem.h>

// Task index stack
tidx task_stack[TASK_NUMBER];
tidx task_stack_index;

// PTB stack
pte32_t* ptb_stack[NUM_PTB];
int ptb_stack_index;

void init_task_stack() {
    // We fill from 0 to TASK_NUMBER - 1
    for (int index = 0; index < TASK_NUMBER; index++) {
        task_stack[index] = index;
    }

    // and we put the index at the start
    task_stack_index = 0;
}

void init_ptb_stack()
{
    // We fill from 0 to NUM_PTB - 1
    for (int i = 0; i < NUM_PTB; i++) {
        ptb_stack[i] = (pte32_t*)(PTB_START + i*PTB_SIZE);
    }

    // and we put the index at the start
    ptb_stack_index = 0;
}

// Pushes the task index onto the stack
void push_task_index(tidx value) {
    // If task_stack_index is already at the bottom, we do nothing
    if (task_stack_index != 0) {
        // We decrement and put the value at the new index
        task_stack[--task_stack_index] = value;
    }
}

// Pushes the ptb onto the stack
void push_ptb(pte32_t* ptb) {
    // If ptb_stack_index is already at the bottom, we do nothing
    if (ptb_stack_index != 0) {
        // We decrement and put the value at the new index
        ptb_stack[--ptb_stack_index] = ptb;
    }
}

// Pops the task index that is ready to use, removing it from the stack. If none returns -1
tidx pop_task_index() {
    // If no value is pointed (after TASK_NUMBER - 1), we return -1
    // Else we return the ptb and we increase the ptb_stack_index
    return task_stack_index == TASK_NUMBER ? -1 : task_stack[task_stack_index++];
}

// Pops the ptb that is ready to use, removing it from the stack. If none returns -1
pte32_t* pop_ptb() {
    // If no value is pointed (after NUM_PTB - 1), we return NULL
    // Else we return the ptb and we increase the ptb_stack_index
    return ptb_stack_index == NUM_PTB ? NULL : ptb_stack[ptb_stack_index++];
}
