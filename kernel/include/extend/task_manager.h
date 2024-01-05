#ifndef __EXTEND_TASK_MANAGER_H__
#define __EXTEND_TASK_MANAGER_H__

#include <types.h>
#include <extend/types.h>
#include <pagemem.h>
#include <extend/memory.h>

#define TASK_NUMBER 10
#define PAGE_NUMBER (USER_END - USER_START + 1) / PAGE_SIZE
#define DEFAULT_QUANTA 5

// Task struct
struct task_t {
    pid task_id;
    offset_t first_page_address;
    offset_t second_page_address;
    uint8_t has_second_page;
    uint8_t quantum;
    uint8_t is_alive;
};

// Inits the task manager
void init_task_manager(void);

// Creates a new task, return 1 if the task couldn't be created
int create_task(void);

// Asks for a second page for the specified task id, returns 1 if fails (eg. already has a second page)
int ask_second_user_page(tidx task_id);

// Ends the task specified by its task id, return 1 if task couldn't be ended (eg. task id do not exist)
int end_task(tidx task_id);

// Returns tasks array 
struct task_t* show_tasks(void);

// Returns tasks array 
tidx current_task(void);

// To call when the IRQ0 (clock) rises
void schedule();

// To switch task, everyone maps this so the CR3 change can happen
void __attribute__((section(".shared_usr_code"),aligned(4))) switch_task(tidx task_id);

#endif