#include <debug.h>

#include <extend/userland.h>
#include <extend/syscall.h>

void __attribute__((section(".task2"),aligned(4))) userland()
{
   debug("This is fine, userland at %p\n", (void*)userland);

   // Syscall
   // int counter = 3;
   int result = read_shared_memory_syscall((int*) 0x2000000);
   debug("Value of read counter = %d\n", result);

   // Priviledged, should provoke an exception
   // asm volatile ("mov %eax, %cr0\n\t");

   while(1);
}
