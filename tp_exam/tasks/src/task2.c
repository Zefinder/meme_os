#include <task2.h>

void __attribute__((section(".task2"),aligned(4))) task2(void)
{
    int *val = (int*)0x4000000;
    while (1) {
        *val = *val + 1;
    }
}
