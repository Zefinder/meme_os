#include <debug.h>
#include <cr.h>
#include <segmem.h>
#include <pagemem.h>

#include <extend/pagemem.h>
#include <extend/segmem.h>
#include <extend/intr.h>
#include <extend/print_utils.h>
#include <extend/userland.h>

#include <task1.h>
#include <task2.h>

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
   // Init the GDT (duh)
   init_gdt();

   // Init the TSS (duh)
   init_tss();

   // Init the IDT (duh)
   init_idt();

   // Init the kernel PGD (duh)
	init_pgd();

   // Jump into ring 3 for testing
   // go_ring_3(userland, USER_END);


   // Let's go testing !
   init_task_manager();
   struct task_t* tasks = show_tasks();

   if (create_task(task1) == 1)
      debug("FUCK\n");

   debug(
      "Task %d:\n"
      "    first page -> 0x%lx\n"
      "    quantum    -> %d\n"
      "    is_alive   -> %d\n",
   tasks->task_id, tasks->first_page_address, tasks->quantum, tasks->is_alive
   );

   if (create_task(task2) == 1)
      debug("FUCK\n");

   debug(
      "Task %d:\n"
      "    first page -> 0x%lx\n"
      "    quantum    -> %d\n"
      "    is_alive   -> %d\n",
   (tasks+1)->task_id, (tasks+1)->first_page_address, (tasks+1)->quantum, (tasks+1)->is_alive
   );

   go_ring_3(userland, USER_END - 0x4);
}
