#include "task_manager.h"

struct task_t {
    uint8_t task_id;
    uint32_t first_page_address;
    uint32_t second_page_address;
    int has_second_page;
};

int create_task() {
    return 3;
}