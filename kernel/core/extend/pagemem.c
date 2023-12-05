#include <extend/pagemem.h>
#include <extend/memory.h>
#include <extend/userland.h>
#include <extend/stacks.h>
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
