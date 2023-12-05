#include <debug.h>

#include <extend/userland.h>
#include <extend/intr.h>

void userland() {
   debug("This is fine.\n");

   // Syscall
   /*
   ** TODO: change asm automatic
   */
   // asm volatile ("mov $1, %eax ; int $128\n\t");
   uint32_t test = 42;
   asm volatile ("mov %0, %%eax ; mov %1, %%ebx ; int %2\n\t" :: "i" (SYS_READ_CNT),"i" () , "i" (INT_SYSCALL) : "%eax", "%ebx");

   // Priviledged, should provoke an exception
   asm volatile ("mov %eax, %cr0\n\t");

   while(1);
}
