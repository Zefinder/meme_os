#include <extend/userland.h>
#include <extend/syscall.h>
#include <extend/timer.h>
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
   int compteur = 0;
   int* compteur_address = (int*) 0x414000;

   // Small hint
   write_stdout_syscall("%s\n", "\n-------------------------------------");
   write_stdout_syscall("%s\n", "\nPress ENTER to switch to command mode\n");
   write_stdout_syscall("%s\n", "-------------------------------------\n");

   // Enable schedule
   start_scheduling_syscall();

   while (1) {
      wait(1);
      write_stdout_syscall("Writing %d to the shared memory...\n", compteur);
      write_shared_memory_syscall(compteur_address, compteur++);
      int result = read_shared_memory_syscall(compteur_address);
      write_stdout_syscall("Value of read counter = %d\n", result);
   }

   // Wait for 10 seconds doing nothing...
   // wait(100);

   // write_stdout_syscall("%s\n", "Killing first task");
   // int task_kill;
   // if ((task_kill = kill_task_syscall()) != 0) {
   //    write_stdout_syscall("Killing first task failed (code=%d)\n", task_kill);
   // }

   // // Show running tasks again before exit
   // show_running_tasks_syscall();

   // Shutdown
   // shutdown_syscall();

   // Priviledged, should provoke an exception
   // asm volatile ("mov %eax, %cr0\n\t");
}
