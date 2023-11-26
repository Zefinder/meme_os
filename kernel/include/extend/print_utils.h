#ifndef PRINT_UTILS_H
#define PRINT_UTILS_H
#include <segmem.h>

void print_gdt_content(gdt_reg_t gdtr_ptr);
void print_gdt(void);
void print_idtr(void);
void print_cr3(void);

#endif