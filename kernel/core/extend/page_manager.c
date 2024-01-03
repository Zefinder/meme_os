#include <extend/page_manager.h>
#include <extend/task_manager.h>
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

// Asks the creation of a PTB for the task, return -1 if couldn't be created. 
void link_ptb(int task_index) {
    // TODO Link PGD to a PTE
    // TOFO If returns 1 then return -1

    // Only for compilation
    task_index++;
    task_index--;
}  

// Will map a page for the current task and return the address, or return -1 if not possible
offset_t map_user_page(int task_index) {
    // If no pages yet, it means that we need to link the PTB to the PGD for the good index
    if (task_pages[task_index].number_user_pages == 0) {
        link_ptb(task_index);
    }

    // Pop address from stack
    offset_t address = pop_page_address();
    
    // If address is NULL then return NULL
    if (address == 0ul) {
        // TODO Unlink PTB
        return 0ul;
    }

    // TODO Put in user PTB (USER_PAGE_ADDR(task_pages[task_index]) as virtual address)
    // TODO Put in kernel PTB (identity mapping)
    // TODO If error return -1

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
    
    // Set back struct to 0
    task_pages[task_index].number_user_pages = 0;
    task_pages[task_index].number_shared_pages = 0;

    return 0;
}

void init_task_pgd(pid task)
{
	pde32_t* krn_PGD = nth_pgd_gbl(0);
	pde32_t* usr_PGD = nth_user_pgds(task);

	switch (task) {
		case 1:
			usr_identity_map(&usr_PGD[0], (offset_t)task1, (offset_t)task1 + PTE_OFFSET - 1);   // map task1 code for task1
			krn_identity_map(&krn_PGD[0], (offset_t)task1, (offset_t)task1 + PTE_OFFSET - 1);   // map task1 code for Kernel

            usr_forced_map(&usr_PGD[0], 0x2000000, SHARED_START, PTE_OFFSET);                   // map shared memory (1 page)
			break;
		case 2:
			usr_identity_map(&usr_PGD[0], (offset_t)task2, (offset_t)task2 + PTE_OFFSET - 1);   // map task2 code for task2
			krn_identity_map(&krn_PGD[0], (offset_t)task2, (offset_t)task2 + PTE_OFFSET - 1);   // map task2 code for Kernel

            usr_forced_map(&usr_PGD[0], 0x4000000, SHARED_START, PTE_OFFSET);                   // map shared memory (1 page)
			break;
		default:
			break;
	}

    krn_identity_map(&krn_PGD[0], SHARED_START, SHARED_END + PTE_OFFSET - 1); // map shared usr page for Kernel
}

void flush_ptb(pte32_t *usr_ptb, pte32_t *krn_ptb)
{
	for(int i = 0; i < 1024; i++) {
		if(usr_ptb[i].p) {
			//__clear_page(usr_ptb[i].addr << 12);
			__clear_page(&usr_ptb[i]);
			__clear_page(&krn_ptb[i]);
		}
	}
}

void flush_pgd(pde32_t *usr_pgd, pde32_t *krn_pgd)
{
	for(int i = 0; i < 1024; i++) {
		if(usr_pgd[i].p) {
			flush_ptb((pte32_t*)(usr_pgd[i].addr << 12), (pte32_t*)(krn_pgd[i].addr << 12));
			__clear_page(&usr_pgd[i]);
			__clear_page(&krn_pgd[i]);

            push_ptb(&usr_pgd[i]);  // put the ptb back into the available ptbs
		}
	}
}

void clear_task_pagemem(pid task)
{
	pde32_t *krn_PGD = nth_pgd_gbl(0);
	pde32_t *usr_PGD = nth_user_pgds(task);

	flush_pgd(krn_PGD, usr_PGD);
}
