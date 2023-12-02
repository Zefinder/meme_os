#include <debug.h>

#include <extend/userland.h>

void userland() {
   debug("This is fine.\n");

   // Syscall
   asm volatile ("mov $1, %eax ; int $48\n\t");

   // Priviledged, should provoke an exception
   asm volatile ("mov %eax, %cr0\n\t");

   while(1);
}
