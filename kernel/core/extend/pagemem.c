#include <extend/pagemem.h>
#include <extend/memory.h>
#include <extend/userland.h>
#include <extend/stacks.h>
#include <task1.h>
#include <task2.h>
#include <debug.h>

void init_pgd()
{
    pde32_t* PGD = nth_pgd_gbl(0);
	init_ptb_stack();


	/**********************************************************************************/
	/*************** ALL MAPPINGS HERE ARE FOR KERNEL PGD AND PTBs ONLY ***************/
	/**********************************************************************************/


	/*************** Mapping addresses and setting up PGD and PTBs ***************/
	// Mapping kernel memory
	krn_identity_map(&PGD[0], KERNEL_START, KERNEL_END);

	// Mapping kernel stack
	krn_identity_map(&PGD[0], KSTACK_START, KSTACK_END);

	// Mapping GDT
	krn_identity_map(&PGD[0], GDT_START, GDT_END);

	// Mapping TSS
	krn_identity_map(&PGD[0], TSS_START, TSS_END);

	// Mapping memory for PGDs
	krn_identity_map(&PGD[0], PGD_START, PGD_START + NUM_PGD*PGD_SIZE - 1);

	// Mapping memory for PTBs
	krn_identity_map(&PGD[0], PTB_START, PTB_START + NUM_PTB*PTB_SIZE - 1);

	// Mapping memory where PGD is located so it can be accessed
	// krn_forced_map(&PGD[0], 0xc0000000, &PGD[0], PGD_SIZE);

	/*************** For testing only: map for user for testing ring 3 jump ***************/
	/*********************  TODO: Should ultimately be kernel mapped  *********************/
	// Mapping user memory
	usr_identity_map(&PGD[0], USER_START, USER_END); // USR

	// Mapping userland, for now userland is in Kernel memory
	usr_identity_map(&PGD[0], (offset_t)userland, (offset_t)userland + PTE_OFFSET - 1);
	usr_identity_map(&PGD[0], (offset_t)printf, (offset_t)userland + PTE_OFFSET - 1);


	/*************** Set up registers ***************/
	// Load PGD into CR3
	cr3_reg_t cr3;
	cr3_pgd_set(&cr3, &PGD[0]);
	set_cr3(cr3);

    // Enable paging in CR0
	cr0_reg_t cr0;
	cr0.raw = get_cr0();
	cr0_pg_en(&cr0);
	set_cr0(cr0);
}

void init_task_pagemem(tidx task)
{
	pde32_t* krn_PGD = nth_pgd_gbl(0);
	pde32_t* usr_PGD = nth_user_pgds(task);

	switch (task) {
		case 0:
			usr_identity_map(&usr_PGD[0], (offset_t)task1, (offset_t)task1 + PTE_OFFSET - 1);   // map task1 code for task1
			krn_identity_map(&krn_PGD[0], (offset_t)task1, (offset_t)task1 + PTE_OFFSET - 1);   // map task1 code for Kernel

            usr_forced_map(&usr_PGD[0], 0x2000000, SHARED_START, PTE_OFFSET);                   // map shared memory (1 page)
			break;
		case 1:
			usr_identity_map(&usr_PGD[0], (offset_t)task2, (offset_t)task2 + PTE_OFFSET - 1);   // map task2 code for task2
			krn_identity_map(&krn_PGD[0], (offset_t)task2, (offset_t)task2 + PTE_OFFSET - 1);   // map task2 code for Kernel

            usr_forced_map(&usr_PGD[0], 0x4000000, SHARED_START, PTE_OFFSET);                   // map shared memory (1 page)
			break;
		default:
			break;
	}

    krn_identity_map(&krn_PGD[0], SHARED_START, SHARED_END + PTE_OFFSET - 1); // map shared usr page for Kernel
}

void task_page_forced_map(tidx task, offset_t virtual_address, offset_t physical_address)
{
	pde32_t* krn_pgd = nth_pgd_gbl(0);
	pde32_t* usr_pgd = nth_user_pgds(task);

	// Put in user ptb
    usr_forced_map(usr_pgd, virtual_address, physical_address, PTE_OFFSET);

    // Put in kernel ptb
    krn_identity_map(krn_pgd, physical_address, physical_address + PTE_OFFSET);

}

void flush_ptb(pte32_t *usr_ptb, pte32_t *krn_ptb)
{
	for(int i = 0; i < 1024; i++) {
		if(usr_ptb[i].p) {
			//__clear_page(usr_ptb[i].addr << 12);
			__clear_page(&usr_ptb[i]);
			__clear_page(&krn_ptb[i]);

			push_ptb(&usr_ptb[i]);  // put the ptb back into the available ptbs
			push_ptb(&krn_ptb[i]);  // put the ptb back into the available ptbs
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
		}
	}
}

void clear_task_pagemem(tidx task)
{
    /*
    *   TODO: this currently clears the shared page for kernel too, later this should wait for both tasks to clear
    */
	pde32_t *krn_PGD = nth_pgd_gbl(0);
	pde32_t *usr_PGD = nth_user_pgds(task);

	flush_pgd(krn_PGD, usr_PGD);
}
