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
/**     _task_ : ring 3 task code address      **/
/**     _esp_  : stack pointer for new task    **/
/************************************************/
// go_ring_3(userland, USER_END - 0x4);
#define go_ring_3(_task_,_esp_)                                                  \
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
         : "i" (d3_sel), "i" (_esp_), "i" (c3_sel), "r" (_task_)                 \
      );                                                                   		\
   }


void tp() {

   // offset_t ebp = get_ebp();
   // offset_t esp = get_esp();

   offset_t offset = (KSTACK_END - 0x4) - get_ebp();
   offset_t new_ebp = offset + get_ebp();
   offset_t new_esp = offset + get_esp();

   set_esp( new_esp );
   set_ebp( new_ebp );

   // Init the GDT (duh)
   init_gdt();

   // Init the TSS (duh)
   init_tss();

   // Init the IDT (duh)
   init_idt();

   // Init the kernel PGD (duh)
	init_pgd();

   // Init task manager (duh)
   init_task_manager();

   // Launching into usermode
   go_ring_3(userland, USER_END - 0x4);
}
