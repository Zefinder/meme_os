#include <debug.h>

#include <task1.h>
#include <extend/intr.h>

void __attribute__((section(".task1"),aligned(4))) task1(void)
{
   while (1) {
      debug("This is fine, task1 at %p\n", (void*)task1);

      // Syscall
      int read_cnt = SYS_READ_CNT;
      int *val = (int*)0x2000000;
      asm volatile ("int %2\n\t" :: "a" (read_cnt), "b" (val) , "i" (INT_SYSCALL));

      // Priviledged, should provoke an exception
      asm volatile ("mov %eax, %cr0\n\t");
   }
}
