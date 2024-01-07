#include <task1.h>

#include <extend/syscall.h>
#include <extend/timer.h>

void __attribute__((section(".task1"),aligned(4))) task1(void)
{
    int counter_address = 0x2000000;
    int x = 0;

    while(1) {
        wait(1);
        x++;
        write_shared_memory_syscall((int*) counter_address, x);
    }
}
