#include <task1.h>
#include <extend/intr.h>

#include <extend/syscall.h>
#include <extend/timer.h>

void __attribute__((section(".task1"),aligned(4))) task1(void)
{
    while(1) {
        wait(1);
        int *counter_address = (int*)0x2000000;
        int counter = read_shared_memory_syscall(counter_address);
        write_stdout_syscall("Current value of counter: %d\n", counter);
    }
}
