#ifndef __TASK_MANAGER__
#define __TASK_MANAGER__

#include <types.h>

// Task struct
struct task_t {
    uint8_t task_id;
    uint32_t first_page_address;
    uint32_t second_page_address;
    int has_second_page;
};

void create_task();

#endif