#include <extend/userland.h>
#include <extend/intr.h>

void __attribute__((section(".task2"),aligned(4))) userland()
{
   int toto = 255;
   int *val = &toto;

   // Syscall
   int read_cnt = SYS_READ_CNT;
   asm volatile ("int %2\n\t" :: "a" (read_cnt), "b" (val) , "i" (INT_SYSCALL));

   // Priviledged, should provoke an exception
   asm volatile ("mov %eax, %cr0\n\t");

   while(1);
}
