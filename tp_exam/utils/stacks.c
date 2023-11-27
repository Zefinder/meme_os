#include "stacks.h"
#include "../tasks/task_manager.h"
#include <debug.h>

struct task_node task_stack;

// Pushes the task into the stack
void push_task(struct task_t value) {
    debug("Task_stack address: %p\n", &task_stack);
    debug("Value:task_id=%d,first_page_address=0x%x,second_page_address=0x%x,has_second_page=%d\n", 
    value.task_id, value.first_page_address, value.second_page_address, value.has_second_page);
}

// Pops the task, removing it from the stack. Returns the pointer of the task variable
struct task_t* pop_task() {
    debug("%p", &task_stack);
    return NULL;
}