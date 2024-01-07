#ifndef __EXTEND_SYSCALL__
#define __EXTEND_SYSCALL__

// Syscalls, move later to other file of defines
#define CREATE_NEW_TASK_SYSCALL     0
#define READ_SHARED_SYSCALL         6
#define WRITE_SHARED_SYSCALL        7

// Syscall0
void create_new_task_syscall();

// Syscall6
int read_shared_memory_syscall(int* address);

// Syscall7
void write_shared_memory_syscall(int* address, int value);

#endif