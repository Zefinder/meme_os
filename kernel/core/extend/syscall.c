#include <extend/syscall.h>
#include <extend/intr.h>

// Syscall0
void create_new_task_syscall() {
}

// Syscall6
int read_shared_memory_syscall(int* address) {
    int result;
    // Result in edx
    asm volatile ("int %3\n\t" : "=d"(result) : "a" (READ_SHARED_SYSCALL), "b" (address) , "i" (INT_SYSCALL)); 
    return result;
}

// Syscall7
void write_shared_memory_syscall(int* address, int value) {
    address = address;
    value = value;
}