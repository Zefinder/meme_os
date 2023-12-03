#ifndef __EXTEND_STACKS_H__
#define __EXTEND_STACKS_H__
#include <extend/task_manager.h>
#include <pagemem.h>

#include <types.h>

// Inits the task stack
void init_task_stack();

// Inits the ptb stack
void init_ptb_stack();

// Inits the page stack
void init_page_stack();

// Pushes the task index into the stack
void push_task_index(tidx task_index);

// Pushes the ptb into the stack
void push_ptb(pte32_t* ptb);

// Pushes the page address into the stack
void push_page_address(offset_t address);

// Pops the task index that is ready to use, removing it from the stack. If none returns -1
tidx pop_task_index();

// Pops the ptb that is ready to use, removing it from the stack. If none returns -1
pte32_t* pop_ptb();

// Pops the page address that is ready to use, removing it from the stack. If none returns -1
offset_t pop_page_address();

#endif
