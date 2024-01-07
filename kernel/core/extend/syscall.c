#include <print.h>

#include <extend/syscall.h>
#include <extend/intr.h>

// Syscall0
int create_new_task_syscall(void* task_address) {
    int creation_value;
    // Result in edx
    asm volatile ("int %3\n\t" : "=d"(creation_value) : "a" (CREATE_NEW_TASK_SYSCALL), "b" (task_address) , "i" (INT_SYSCALL));
    return creation_value;
}

// Syscall2
void write_stdout_syscall(const char *format, ...) {
    va_list params;
    va_start(params, format);
    asm volatile ("int %3\n\t" :: "a" (WRITE_STDOUT_SYSCALL), "b" (format), "c" (&params) , "i" (INT_SYSCALL));
    va_end(params);
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
    asm volatile ("int %3\n\t" :: "a" (WRITE_SHARED_SYSCALL), "b" (address), "c" (value) , "i" (INT_SYSCALL)); 
}

// Syscall8
void show_running_tasks_syscall() {
    asm volatile ("int %1\n\t" :: "a" (SHOW_RUNNING_TASKS) , "i" (INT_SYSCALL)); 
}