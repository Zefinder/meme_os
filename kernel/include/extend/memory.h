#ifndef MEMORY_H
#define MEMORY_H

#include <segmem.h>

// Hard memory mapping
#define KERNEL_START	0x300000
#define KERNEL_END		0x33ffff


#define KSTACK_START	0x340000
#define KSTACK_END		0x34ffff


// Page related constants
#define PAGE_START		0x350000
#define PAGE_END		0x366fff

#define PGD_START       0x350000
#define PGD_END         0x35afff
#define PGD_SIZE        0x001000
#define PGD_OFFSET      0x001000

#define PTB_START       0x35b000
#define PTB_END         0x366fff
#define PTB_SIZE        0x001000
#define PTB_OFFSET      0x000010


#define USER_START 		0x400000
#define USER_END		0X413fff


// Segment related constants
#define GDT_SIZE    6
#define GDT_START   0x370000
#define GDT_LIMIT   GDT_SIZE*8 - 1
#define GDT_END     GDT_START + GDT_LIMIT

#define TSS_START   0x380000
#define TSS_LIMIT   sizeof(tss_t)
#define TSS_END     TSS_START + TSS_LIMIT


#define SHARED_START	0x414000
#define SHARED_END		0x4fffff

#endif