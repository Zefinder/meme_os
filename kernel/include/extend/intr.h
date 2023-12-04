#ifndef INTR_H
#define INTR_H

// Interrupts
#define INT_SYSCALL     0x80

// Syscalls, move later to other file of defines
#define SYS_READ_CNT    1

void init_idt(void);

#endif
