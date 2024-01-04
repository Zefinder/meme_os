#include <extend/page_manager.h>
#include <extend/task_manager.h>
#include <extend/pagemem.h>
#include <extend/stacks.h>
#include <string.h>

static struct task_pages_t task_pages[TASK_NUMBER];

// Inits the page manager
void init_page_manager() {
    // Inits page address stack
    init_page_stack();

    // Put task_pages to 0
    memset(task_pages, 0, sizeof(task_pages));
}

// Will map a page for the current task and return the address, or return 0 if not possible
offset_t map_user_page(int task_index) {
    // Pop address from stack
    offset_t address = pop_page_address();
    
    // If address is 0 then return 0 and nothing else since nothing happened here
    if (address == 0ul) {
        return 0ul;
    }

    // Add address to task PTB
    task_page_forced_map(task_index, USER_PAGE_ADDRESS(task_pages[task_index].number_user_pages), address);

    // Add 1 to allocated pages
    task_pages[task_index].number_user_pages++;

    // Else return address
    return address;
}

// Frees all pages for task, returns 1 if an error occured
int free_task_pages(struct task_t task, int task_index) {
    // Repush user page addresses
    push_page_address(task.first_page_address);
    if (task.has_second_page) {
        push_page_address(task.second_page_address);
    }

    // TODO Remove shared pages for everyone
    // TODO Repush shared pages
    // TODO Tell PGD and PTB to flush their entries
    clear_task_pagemem(task_index);
    
    // Set back struct to 0
    task_pages[task_index].number_user_pages = 0;
    task_pages[task_index].number_shared_pages = 0;

    return 0;
}
