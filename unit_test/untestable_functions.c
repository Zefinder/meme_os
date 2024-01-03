// THIS IS A TEST FEATURE DO NOT REPRODUCE IT AT HOME
// We create functions that are kernel based and cannot be tested
#include <extend/types.h>
#include <types.h>
void init_task_pagemem(tidx task) {}
void task_page_forced_map(tidx task, offset_t virtual_address, offset_t physical_address) {}
void clear_task_pagemem(tidx task) {}