#include <debug.h>
#include <cr.h>
#include <pagemem.h>
#include <extend/pagemem.h>
#include <extend/segmem.h>
#include <extend/print_utils.h>


/************************************************/
/**        Goes into ring 3 (usermode)         **/
/**                                            **/
/**     _esp_ : stack pointer for new task     **/
/************************************************/
#define go_ring_3(_esp_)                                                        	\
   {                                                                       		\
      set_ds(d3_sel);                                                      		\
      set_es(d3_sel);                                                      		\
      set_fs(d3_sel);                                                      		\
      set_gs(d3_sel);                                                      		\
      TSS.s0.esp  = _esp_;                                             		      \
      TSS.s0.ss   = d0_sel;                                                		\
      TSS.ds      = d0_sel;                                                		\
      TSS.es      = d0_sel;                                                		\
      TSS.fs      = d0_sel;                                                		\
      TSS.gs      = d0_sel;                                                		\
      set_tr(ts_sel);                                                      		\
      asm volatile (                                                       		\
         "push %0\n"                                                       		\
         "push %1\n"                                                       		\
         "pushf\n"                                                         		\
         "push %2\n"                                                       		\
         "push %3\n"                                                       		\
         "iret\n"                                                          		\
         :                                                                 		\
         : "i" (d3_sel), "i" (USER_START+0x14), "i" (c3_sel), "r" (userland)     \
      );                                                                   		\
   }


void tp() {
   init_gdt();

   gdt_reg_t gdtr_ptr;
   get_gdtr(gdtr_ptr);
   print_gdt_content(gdtr_ptr);

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
	
   pte32_t* PTB = nth_ptb(0);
	debug("Accès PTB réussi ! -> PTB[0] = %08x\n", PTB[0].raw);
	debug("Accès PGD via 0xc0000000 réussi ! -> PGD[0] = %08x\n", ((pde32_t*)0xc0000000)[0].raw);

	// (Q8)
	debug("Chaîne de caractères à 0x2000 via %08x : %s\n", 0x700000, (char*)0x700000);
	debug("Chaîne de caractères à 0x2000 via %08x : %s\n", 0x7ff000, (char*)0x7ff000);

	// (Q9)
	((pde32_t*)0xc0000000)->raw = 0ull;
}