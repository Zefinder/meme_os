#include <debug.h>
#include <cr.h>
#include <segmem.h>
#include <pagemem.h>

#include <extend/pagemem.h>
#include <extend/segmem.h>
#include <extend/print_utils.h>
#include <extend/userland.h>

#include <intr.h>

/************************************************/
/**        Goes into ring 3 (usermode)         **/
/**                                            **/
/**     _esp_ : stack pointer for new task     **/
/************************************************/
#define go_ring_3(_esp_)                                                        	\
   {                                                                       		\
      set_ds(d3_sel);                                                            \
      set_es(d3_sel);                                                            \
      set_fs(d3_sel);                                                            \
      set_gs(d3_sel);                                                            \
      asm volatile (                                                       		\
         "push %0\n"                                                       		\
         "push %1\n"                                                       		\
         "pushf\n"                                                         		\
         "push %2\n"                                                       		\
         "push %3\n"                                                       		\
         "iret\n"                                                          		\
         :                                                                 		\
         : "i" (d3_sel), "i" (_esp_), "i" (c3_sel), "r" (userland)               \
      );                                                                   		\
   }


void tp() {
   debug("intr: %p\n", intr_hdlr);

   init_gdt();
   // Print gdt to verify good execution
   print_gdt();

   init_tss();

	init_pgd();

   // Print CR3 to verify good execution
	cr3_reg_t cr3;
	cr3.raw = (uint32_t)get_cr3();

	debug(
		"\nCR3 :\n"
		"	PWT 	-> %d\n"
		"	PCD 	-> %d\n"
		"	PD Base -> 0x%08x\n\n",
		cr3.pwt, cr3.pcd, cr3.addr<<12
		);
	
   // pte32_t* PTB = nth_ptb(0);
	// debug("Accès PTB réussi ! -> PTB[0] = 0x%08x\n", PTB[0].raw);
	// debug("Accès PGD via 0xc0000000 réussi ! -> PGD[0] = 0x%08x\n", ((pde32_t*)0xc0000000)[0].raw);

   go_ring_3(USER_END);

}
