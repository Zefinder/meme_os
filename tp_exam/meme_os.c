#include <debug.h>
#include <cr.h>
#include <segmem.h>
#include <pagemem.h>

#include <extend/pagemem.h>
#include <extend/segmem.h>
#include <extend/intr.h>
#include <extend/print_utils.h>
#include <extend/userland.h>

// #include <intr.h>

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
   // Init the GDT (duh)
   init_gdt();

   // Init the TSS (duh)
   init_tss();

   // Init the IDT (duh)
   init_idt();

   // Init the PGD (duh)
	init_pgd();

   // Jump into ring 3 for testing
   go_ring_3(USER_END);
}
