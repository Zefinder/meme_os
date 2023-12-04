#include <extend/stacks.h>
#include <extend/task_manager.h>
#include <string.h>

static struct task_t running_tasks[TASK_NUMBER];
static int scheduling_task_index;
static uint64_t next_task_id;

void init_scheduling(void) {
    scheduling_task_index = 0;
}

// Inits the task manager
void init_task_manager(void) {
    // We init the available stack index
    init_task_stack();

    // We init scheduling
    init_scheduling();

    // Resetting all tasks
    memset(running_tasks, 0, sizeof(struct task_t) * TASK_NUMBER);

    // Next task id is 0
    next_task_id = 0;
}

// Creates a new task, return 1 if the task couldn't be created
int create_task(void) {
    // Pop task index
    int task_index = pop_task_index();

    // If index is -1 then we return 1
    if (task_index == -1) {
        return 1;
    }

    // TODO Else we ask for a user page
    // If address is -1 then repush the task index and we return 1
    // TODO Add in good index PTB

    // We create a task struct with the address within the good index
    running_tasks[task_index].task_id = next_task_id;
    running_tasks[task_index].first_page_address = 0x400000;
    running_tasks[task_index].second_page_address = 0;
    running_tasks[task_index].has_second_page = 0;
    running_tasks[task_index].quantum = DEFAULT_QUANTA;
    running_tasks[task_index].is_alive = 1;

    // Add 1 to task count
    next_task_id++;

    return 0;
}

// Asks for a second page for the specified task id, returns 1 if fails (eg. already has a second page)
int ask_second_user_page(uint64_t task_id) {
    // Go through all running pages
    for (int task_index = 0; task_index < TASK_NUMBER; task_index++) {
        // If found we check if it already has a second page
        if (running_tasks[task_index].task_id == task_id && running_tasks[task_index].is_alive) {
            // If already has second page, returns 1
            if (running_tasks[task_index].has_second_page) {
                return 1;
            }

            // TODO Else asks for a user page
            // TODO If address is -1 then return 1
            // TODO Add in PTB

            // Add page and address to task
            running_tasks[task_index].second_page_address = 0x401000;
            running_tasks[task_index].has_second_page = 1;
            
            return 0;
        }
    }    

    // If not found then return 1
    return 1;  
}

// Ends the task specified by its task id, return 1 if task couldn't be ended (eg. task id do not exist)
int end_task(uint64_t task_id) {
    // Go through all running tasks
    for (int task_index = 0; task_index < TASK_NUMBER; task_index++) {
        if (running_tasks[task_index].task_id == task_id && running_tasks[task_index].is_alive) {
            // If found and alive, we put is_alive to 0
            running_tasks[task_index].is_alive = 0;

            // TODO We free the first page
            // TODO If it has a second page we also free it
            if (running_tasks[task_index].has_second_page) {

            }

            // TODO Invalidate PTB entries

            // We push back the task index
            push_task_index(task_index);

            // We return 0
            return 0;
        }
    }

    // If not found then return 1
    return 1;
}

// Returns tasks array 
struct task_t* show_tasks(void) {
    return running_tasks;
}

tidx current_task(void) {
    return scheduling_task_index;
}