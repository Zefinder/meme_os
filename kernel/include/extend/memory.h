#ifndef MEMORY_H
#define MEMORY_H

#include <segmem.h>
#include <extend/task_manager.h>

// Hard memory mapping
#define KERNEL_START	0x300000    // Start address of Kernel memory
#define KERNEL_END		0x33ffff    // End address of Kernel memory

#define KSTACK_START	0x340000    // Start address of Kernel stack
#define KSTACK_END		0x34ffff    // End address of Kernel stack

#define USER_START 		0x400000    // Start address of User memory
#define USER_END		0X413fff    // End address of User memory

#define SHARED_START	0x414000    // Start address of shared User memory
#define SHARED_END		0x4fffff    // End address of shared User memory


// Page related constants
#define PAGE_START		0x350000    // Start address of paging related memory

#define NUM_PGD         (TASK_NUMBER + 1)                                   // 1 PGD per task + Kernel
#define PGD_SIZE        0x001000                                            // Size in memory of 1 PGD
#define PGD_START       PAGE_START                                          // Start address of the first PGD (and thus all the PGDs)
#define PGD_END         (PGD_START + NUM_PGD*PGD_SIZE - 1)                  // End address of the first PGD (and thus all the PGDs)
#define PGD_OFFSET      0x100000000                                         // Memory that can be handled by one PGD (4GB)
#define PDE_OFFSET      0x400000                                            // Address offset that provokes a change in PDE (used for mapping)

#define NUM_PTB         20                                                  // Allocated at mapping
#define PTB_SIZE        0x001000                                            // Size in memory of 1 PTB
#define PTB_START       (PGD_END + 1)                                       // Start address of the first PTB (and thus all the PTBs)
#define PTB_END         (PTB_START + NUM_PTB*PTB_SIZE - 1)                  // End address of the first PTB (and thus all the PTBs)
#define PTB_OFFSET      PDE_OFFSET                                          // Memory that can be handled by one PTB (4MB)
#define PTE_OFFSET      0x001000                                            // Address offset that provokes a change in PTE (used for mapping)

#define PAGE_END		PTB_END     // End address of paging related memory


// Segment related constants
#define GDT_SIZE    6                       // Number of segments (first one is NULL)
#define GDT_START   0x370000                // Start address of GDT
#define GDT_LIMIT   (GDT_SIZE*8 - 1)        // Size in Bytes of GDT
#define GDT_END     (GDT_START + GDT_LIMIT) // End address of GDT

#define TSS_START   0x380000                // Start address of TSS
#define TSS_LIMIT   (sizeof(tss_t) - 1)     // Size in Bytes of TSS
#define TSS_END     (TSS_START + TSS_LIMIT) // End address of TSS

#endif
