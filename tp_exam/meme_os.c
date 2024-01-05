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
/**     _fun_ : ring 3 code                    **/
/**     _esp_ : stack pointer for new task     **/
/************************************************/
#define go_ring_3(_esp_)                                                         \
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

   // Init the kernel PGD (duh)
	init_pgd();

   // Jump into ring 3 for testing
   // go_ring_3(userland, USER_END);


   // Let's go testing !
   // init_task_manager();
   // struct task_t* tasks = show_tasks();

   // if (create_task() == 1)
   //    debug("FUCK\n");

   // debug(
   //    "Task %d:\n"
   //    "    first page -> 0x%lx\n"
   //    "    quantum    -> %d\n"
   //    "    is_alive   -> %d\n",
   // tasks->task_id, tasks->first_page_address, tasks->quantum, tasks->is_alive
   // );

   // if (create_task() == 1)
   //    debug("FUCK\n");

   // debug(
   //    "Task %d:\n"
   //    "    first page -> 0x%lx\n"
   //    "    quantum    -> %d\n"
   //    "    is_alive   -> %d\n",
   // (tasks+1)->task_id, (tasks+1)->first_page_address, (tasks+1)->quantum, (tasks+1)->is_alive
   // );

	// cr3_reg_t cr3;
   // pde32_t *task1_PGD = (pde32_t *)nth_user_pgds(0);
	// cr3_pgd_set(&cr3, &task1_PGD[0]);
	// set_cr3(cr3);

   debug("Userland at %p\n", (void*)userland);

   go_ring_3(USER_END);

}
