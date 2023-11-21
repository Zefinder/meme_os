#include <extend/pagemem.h>
#include <extend/memory.h>


void init_pgd()
{
    pde32_t* PGD = nth_pgd(0);
    pte32_t* PTB = nth_ptb(0);

	// Mapping kernel memory
	for(offset_t p = KERNEL_START; p <= KERNEL_END; p += PGD_OFFSET) {
		pgd_identity_pde(&PGD[0], &PTB[0], p);
	}
	for(offset_t p = KERNEL_START; p <= KERNEL_END; p += PTB_OFFSET) {
		ptb_identity_pte(&PTB[0], p);
	}

	// Mapping memory for PTB
	pgd_identity_pde(&PGD[0], &PTB[0], &PTB[0]);
	for(offset_t p = (offset_t)&PTB[0]; p < (offset_t)&PTB[0] + PTB_SIZE; p += PTB_OFFSET) {
		ptb_identity_pte(&PTB[0], p);
	}

	// Mapping memory where PGD is located so it can be accessed
	pgd_identity_pde(&PGD[0], &PTB[0], 0xc0000000);
	ptb_forced_pte(&PTB[0], 0xc0000000, (offset_t)&PGD[0]);


	// (Q8)
	pgd_identity_pde(&PGD[0], &PTB[0], 0x700000);
	ptb_forced_pte(&PTB[0], 0x700000, 0x2000);
	ptb_forced_pte(&PTB[0], 0x7ff000, 0x2000);

	cr3_reg_t cr3;
	cr3_pgd_set(&cr3, &PGD[0]);
	set_cr3(cr3);

    // Enable paging
	cr0_reg_t cr0;
	cr0.raw = get_cr0();
	cr0_pg_en(&cr0);
	set_cr0(cr0);
}