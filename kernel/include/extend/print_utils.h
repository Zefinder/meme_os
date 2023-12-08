#ifndef __EXTEND_PRINT_UTILS_H__
#define __EXTEND_PRINT_UTILS_H__
#include <segmem.h>

void print_gdt_content(gdt_reg_t gdtr_ptr);
void print_gdt(void);
void print_idtr(void);
void print_cr3(void);

#endif
