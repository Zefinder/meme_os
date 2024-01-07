#include <extend/stacks.h>
#include <extend/task_manager.h>
#include <extend/page_manager.h>
#include <extend/pagemem.h>
#include <extend/segmem.h>
#include <string.h>

static struct task_t running_tasks[TASK_NUMBER];
static int current_task_index;
static uint64_t next_task_id;

void init_scheduling(void) {
    current_task_index = 0;
}

// Inits the task manager
void init_task_manager(void) {
    // We init the available stack index
    init_task_stack();

    // We init the page manager
    init_page_manager();

    // We init scheduling
    init_scheduling();

    // Resetting all tasks
    memset(running_tasks, 0, sizeof(struct task_t) * TASK_NUMBER);

    // Next task id is 0
    next_task_id = 0;
}

// Creates a new task, return 1 if the task couldn't be created
int create_task(void *task) {
    // Pop task index
    tidx task_index = pop_task_index();

    // Init PGD for task and kernel
    init_task_pagemem(task_index, task);

    // If index is -1 then we return 1
    if (task_index == -1) {
        return 1;
    }

    // Else we ask for a user page
    offset_t first_page_index = 1; // map_user_page(task_index);
    
    // If address is 0 then repush the task index and we return 1
    if (first_page_index == 0ul) {
        clear_task_pagemem(task_index);
        return 1;
    }
    
    // We create a task struct with the address within the good index
    running_tasks[task_index].task_id = next_task_id;
    running_tasks[task_index].first_page_address = pop_page_address();;
    running_tasks[task_index].second_page_address = 0;
    running_tasks[task_index].has_second_page = 0;
    running_tasks[task_index].quantum = DEFAULT_QUANTA;
    running_tasks[task_index].is_alive = 1;

    // Creating first context
    running_tasks[task_index].ctx.cs.raw        = c3_sel;
    running_tasks[task_index].ctx.ss.raw        = d3_sel;
    running_tasks[task_index].ctx.eip.raw       = (offset_t)task;
    running_tasks[task_index].ctx.esp.raw       = running_tasks[task_index].first_page_address + PAGE_SIZE - 0x4; // 4 bytes for the previous ebp
    running_tasks[task_index].ctx.gpr.ebp.raw   = running_tasks[task_index].first_page_address + PAGE_SIZE - 0x4;
    running_tasks[task_index].ctx.gpr.esp.raw   = running_tasks[task_index].first_page_address + PAGE_SIZE - 0x4;

    // Add 1 to task count
    next_task_id++;

    return 0;
}

// Asks for a second page for the specified task id, returns 1 if fails (eg. already has a second page)
int ask_second_user_page(tidx task_id) {
    // Go through all running pages
    for (int task_index = 0; task_index < TASK_NUMBER; task_index++) {
        // If found we check if it already has a second page
        if (running_tasks[task_index].task_id == task_id && running_tasks[task_index].is_alive) {
            // If already has second page, returns 1
            if (running_tasks[task_index].has_second_page) {
                return 1;
            }

            // Else asks for a user page
            offset_t second_page_address = map_user_page(task_index);

            // If address is 0 then return 1
            if(second_page_address == 0ul) {
                return 1;
            }

            // Add page and address to task
            running_tasks[task_index].second_page_address = second_page_address;
            running_tasks[task_index].has_second_page = 1;
            
            return 0;
        }
    }    

    // If not found then return 1
    return 1;  
}

// Ends the task specified by its task id, return 1 if task couldn't be ended (eg. task id do not exist)
int end_task(tidx task_id) {
    // If the index is alive then we end it
    if (running_tasks[task_id].is_alive) {
        // If found and alive, we put is_alive to 0
        running_tasks[task_id].is_alive = 0;

        // We free user pages
        free_task_pages(running_tasks[task_id], task_id);

        // We push back the task index
        push_task_index(task_id);

        // We return 0
        return 0;
    }

    // Else return 1
    return 1;
}

// Returns tasks array 
struct task_t* show_tasks(void) {
    return running_tasks;
}

tidx current_task(void) {
    return current_task_index;
}

tidx next_task() {
    // Searching next schedulable task
    int new_task_index = (current_task_index + 1) % TASK_NUMBER;

    while
    (
        new_task_index != (current_task_index)      // Stop condition
        && !running_tasks[new_task_index].is_alive  // Task is schedulable ?
    )
    {
        new_task_index = (new_task_index + 1) % TASK_NUMBER;
    }
    
    return new_task_index;
}

int_ctx_t *schedule()
{
    struct task_t *current_task = &running_tasks[current_task_index];

    if (!current_task->is_alive) {
        current_task_index = next_task();
    } else {
        current_task->quantum--;

        if (current_task->quantum == 0) {
            current_task->quantum = DEFAULT_QUANTA;
            current_task_index = next_task();
        }
    }

    current_task = &running_tasks[current_task_index];

    if (!current_task->is_alive) {
        // If there are no schedulable tasks
        return 0ul;
    }

    return &(current_task->ctx);
}

void __attribute__((section(".shared_usr_code"),aligned(4))) load_task_pgd(tidx task_id)
{
    // Here we are already ring 3 when we call this, the first ring 0 -> ring 3 is through userland
    // However in CR3 it's the Kernel's PGD, loaded previously from boot or IRQ0 handler
    pde32_t *task_PGD = nth_user_pgds(task_id);

    cr3_reg_t cr3;
    cr3_pgd_set(&cr3, &task_PGD[0]);
    set_cr3(cr3);
}

void __attribute__((section(".shared_usr_code"),aligned(4))) save_task_ctx(tidx task_id, int_ctx_t *ctx)
{
    memcpy(ctx, &(running_tasks[task_id].ctx), sizeof(int_ctx_t));
}
