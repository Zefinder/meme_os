#include <debug.h>

#include <extend/userland.h>
#include <extend/intr.h>

void __attribute__((section(".usr"),aligned(4))) userland()
{
   debug("This is fine, userland at %p\n", (void*)userland);

   // Syscall
   int read_cnt = SYS_READ_CNT;
   int val = 0;
   asm volatile ("int %2\n\t" :: "a" (read_cnt), "b" (val) , "i" (INT_SYSCALL));

   // Priviledged, should provoke an exception
   asm volatile ("mov %eax, %cr0\n\t");

   while(1);
}
