#ifndef __TASK__
#define __TASK__

#include <types.h>

struct task_t {
    uint8_t task_id;
    uint32_t first_address;
    int has_other_page;
};


#endif