#ifndef __STACKS_H__
#define __STACKS_H__
#include <extend/task_manager.h>
#include <pagemem.h>

// Inits the task stack
void init_task_stack();

// Inits the ptb stack
void init_ptb_stack();

// Pushes the task index into the stack
void push_task_index(tidx task_index);

// Pushes the ptb into the stack
void push_ptb(pte32_t* ptb);

// Pops the task index that is ready to use, removing it from the stack. If none returns -1
tidx pop_task_index();

// Pops the ptb that is ready to use, removing it from the stack. If none returns -1
pte32_t* pop_ptb();

#endif
