#include <debug.h>
#include <cr.h>
#include <pagemem.h>
#include <extend/pagemem.h>

// Extrait du TP4

pde32_t* PGD = (pde32_t*)0x600000;
pte32_t* PTB = (pte32_t*)0x601000;

void init_pgd()
{
	// Mapping memory from 0x300000 to 0x340000
	for(offset_t p = 0x300000; p < 0x340000; p += 0x1000) {
		pgd_identity_pde(&PGD[0], &PTB[0], p);
	}
	for(offset_t p = 0x300000; p < 0x340000; p += 0x10) {
		ptb_identity_pte(&PTB[0], p);
	}

	// (Q6) Mapping memory where PTB is located so it can be accessed
	pgd_identity_pde(&PGD[0], &PTB[0], 0x601000);
	for(offset_t p = 0x601000; p < 0x602000; p += 0x10) {
		ptb_identity_pte(&PTB[0], p);
	}

	// (Q7) Mapping memory where PGD is located so it can be accessed
	pgd_identity_pde(&PGD[0], &PTB[0], 0xc0000000);
	ptb_forced_pte(&PTB[0], 0xc0000000, (offset_t)&PGD[0]);


	// (Q8)
	pgd_identity_pde(&PGD[0], &PTB[0], 0x700000);
	ptb_forced_pte(&PTB[0], 0x700000, 0x2000);
	ptb_forced_pte(&PTB[0], 0x7ff000, 0x2000);

	cr3_reg_t cr3;
	cr3_pgd_set(&cr3, &PGD[0]);
	set_cr3(cr3);

	cr0_reg_t cr0;
	cr0.raw = get_cr0();
	cr0_pg_en(&cr0);
	set_cr0(cr0);
}

void tp() {
	init_pgd();

	cr3_reg_t cr3;
	cr3.raw = (uint32_t)get_cr3();

	debug(
		"\nCR3 :\n"
		"	PWT 	-> %d\n"
		"	PCD 	-> %d\n"
		"	PD Base -> 0x%08x\n\n",
		cr3.pwt, cr3.pcd, cr3.addr<<12
		);
	
	debug("Accès PTB réussi ! -> PTB[0] = %08x\n", PTB[0].raw);
	debug("Accès PGD via 0xc0000000 réussi ! -> PGD[0] = %08x\n", ((pde32_t*)0xc0000000)[0].raw);

	// (Q8)
	debug("Chaîne de caractères à 0x2000 via %08x : %s\n", 0x700000, (char*)0x700000);
	debug("Chaîne de caractères à 0x2000 via %08x : %s\n", 0x7ff000, (char*)0x7ff000);

	// (Q9)
	((pde32_t*)0xc0000000)->raw = 0ull;
}