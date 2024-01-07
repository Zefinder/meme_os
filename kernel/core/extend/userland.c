#include <extend/userland.h>
#include <extend/syscall.h>
#include <debug.h>

#include <task1.h>
#include <task2.h>

void __attribute__((section(".task2"),aligned(4))) userland()
{
   // Task creation
   int task_create;
   if ((task_create = create_new_task_syscall(task1)) != 0) {
      write_stdout_syscall("First task creation failed (code=%d)\n", task_create);
   }

   if ((task_create = create_new_task_syscall(task2)) != 0) {
      write_stdout_syscall("Second task creation failed (code=%d)\n", task_create);
   }

   // Show running tasks
   show_running_tasks_syscall();

   // Writing to shared page and reading it
   int compteur = 666;
   int* compteur_address = (int*) 0x414000;
   write_stdout_syscall("Writing %d to the shared memory...\n", compteur);
   write_shared_memory_syscall(compteur_address, compteur);
   int result = read_shared_memory_syscall(compteur_address);
   write_stdout_syscall("Value of read counter = %d\n", result);

   // Priviledged, should provoke an exception
   // asm volatile ("mov %eax, %cr0\n\t");

   while(1);
}
