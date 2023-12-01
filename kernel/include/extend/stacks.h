#ifndef __STACKS__
#define __STACKS__

// Inits the task stack
void init_task_stack();

// Pushes the task index into the stack
void push_task_index(int task_index);

// Pops the task index that is ready to use, removing it from the stack. If none returns -1
int pop_task_index();

#endif