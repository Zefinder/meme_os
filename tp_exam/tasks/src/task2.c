#include <task2.h>
#include <debug.h>
#include <extend/syscall.h>

void __attribute__((section(".task2"),aligned(4))) task2(void)
{
    int counter_address = 0x4000000;
    int counter;

    while(1) {
        counter = read_shared_memory_syscall((int*) counter_address);
        debug("Current value of counter: %d\n", counter);
    }
}
