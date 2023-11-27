#ifndef __STACKS__
#define __STACKS__

struct task_node {
    struct task_t* value;
    struct node* next_node;
};

// Pushes the task into the stack
void push_task(struct task_t value);

// Pops the task, removing it from the stack. Returns the pointer of the task variable
struct task_t* pop_task();

#endif