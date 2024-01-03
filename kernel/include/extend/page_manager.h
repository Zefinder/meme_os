#ifndef __PAGE_MANAGER__
#define __PAGE_MANAGER__

#include <types.h>
#include <extend/task_manager.h>
#include <pagemem.h>

#define USER_PAGE_ADDRESS(_n_) (PG_4K_SIZE * _n_)

struct task_pages_t {
    int number_user_pages;
    int number_shared_pages;
};

// Inits the page manager
void init_page_manager();

// Will map a page for the current task and return the address, or return -1 if not possible
offset_t map_user_page(int task_index);

// Frees all pages for task, returns 1 if an error occured
int free_task_pages(struct task_t task, int task_index);

void init_task_pagemem(pid task);

// Flushes paging entries in usr_ptb and mirrors actions in krn_ptb
void flush_ptb(pte32_t *usr_ptb, pte32_t *krn_ptb);

// Flushes paging entries in usr_pgd and mirrors actions in krn_pgd
void flush_pgd(pde32_t *usr_pgd, pde32_t *krn_pgd);

void clear_task_pagemem(pid task);

#endif
