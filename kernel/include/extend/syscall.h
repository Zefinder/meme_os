#ifndef __EXTEND_SYSCALL__
#define __EXTEND_SYSCALL__

// Syscalls, move later to other file of defines
#define CREATE_NEW_TASK_SYSCALL     0
#define WRITE_STDOUT_SYSCALL        2
#define READ_SHARED_SYSCALL         6
#define WRITE_SHARED_SYSCALL        7
#define SHOW_RUNNING_TASKS          8

// Syscall0
int create_new_task_syscall(void* task_address);

// Syscall2
void write_stdout_syscall(const char *format, ...);

// Syscall6
int read_shared_memory_syscall(int* address);

// Syscall7
void write_shared_memory_syscall(int* address, int value);

// Syscall8
void show_running_tasks_syscall();

#endif