#include <debug.h>

#include <extend/userland.h>

void userland() {
   debug("This is fine.\n");

   // Priviledged, should provoke an exception
   asm volatile ("mov %eax, %cr0");

   while(1);
}
