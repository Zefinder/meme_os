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
   int read_cnt = SYS_READ_CNT;
   int val = VALUE;
   asm volatile ("int %2\n\t" :: "a" (read_cnt), "b" (val) , "i" (INT_SYSCALL));

   // Priviledged, should provoke an exception
   asm volatile ("mov %eax, %cr0\n\t");

   while(1);
}
