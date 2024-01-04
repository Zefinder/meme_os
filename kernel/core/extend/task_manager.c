#include <extend/stacks.h>
#include <extend/task_manager.h>
#include <extend/page_manager.h>
#include <extend/pagemem.h>
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
int create_task(void) {
    // Pop task index
    tidx task_index = pop_task_index();

    // Init PGD for task and kernel
    init_task_pagemem(task_index);

    // If index is -1 then we return 1
    if (task_index == -1) {
        return 1;
    }

    // Else we ask for a user page
    offset_t first_page_index = map_user_page(task_index);
    
    // If address is 0 then repush the task index and we return 1
    if (first_page_index == 0ul) {
        clear_task_pagemem(task_index);
        return 1;
    }
    
    // We create a task struct with the address within the good index
    running_tasks[task_index].task_id = next_task_id;
    running_tasks[task_index].first_page_address = first_page_index;
    running_tasks[task_index].second_page_address = 0;
    running_tasks[task_index].has_second_page = 0;
    running_tasks[task_index].quantum = DEFAULT_QUANTA;
    running_tasks[task_index].is_alive = 1;

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
    // Go through all running tasks
    for (int task_index = 0; task_index < TASK_NUMBER; task_index++) {
        if (running_tasks[task_index].task_id == task_id && running_tasks[task_index].is_alive) {
            // If found and alive, we put is_alive to 0
            running_tasks[task_index].is_alive = 0;

            // We free user pages
            free_task_pages(running_tasks[task_index], task_index);

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

void search_for_new_task() {
        // Create values
        int new_task_index = 0;
        int found = 0;
        while (!found && new_task_index < TASK_NUMBER) {
            if (running_tasks[scheduling_task_index].is_alive) {
                // We found !
                found = 1;
            }
            new_task_index++;
        }

        // If something was found then update index
        if (found) {
            scheduling_task_index = new_task_index;
            // TODO Switch task to this new task
        }
}

void schedule() {
    struct task_t current_task = running_tasks[scheduling_task_index];

    // If no tasks, then on dead task, search for one alive
    if (!current_task.is_alive) {
        search_for_new_task();
    } else {
        // Quantum decrease
        current_task.quantum--;

        // If quantum = 0 then back to default and search for new task
        if (current_task.quantum == 0) {
            current_task.quantum = DEFAULT_QUANTA;
            search_for_new_task();
        }

    }
}