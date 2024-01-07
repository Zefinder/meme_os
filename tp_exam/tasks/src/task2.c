#include <task2.h>
#include <extend/syscall.h>

void __attribute__((section(".task2"),aligned(4))) task2(void)
{
    int counter_address = 0x4000000;
    int counter = 0;

    while(1) {
        Write_shared_memory_syscall((int*) counter_address, counter++);
    }
}
