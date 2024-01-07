#ifndef __EXTEND_MEMORY_H__
#define __EXTEND_MEMORY_H__

#include <segmem.h>
#include <extend/task_manager.h>

// Hard memory mapping
// THE END ADDRESSES ARE THE FIRST ADDRESS NOT IN THE MEMORY SECTION
#define KERNEL_START	        0x300000    // Start address of Kernel memory
#define KERNEL_END		        0x340000    // End address of Kernel memory

#define KSTACK_START	        0x340000    // Start address of Kernel stack
#define KSTACK_END		        0x350000    // End address of Kernel stack

#define USER_START 		        0x400000    // Start address of User memory (stacks and all)
#define USER_END		        0X414000    // End address of User memory (stacks and all)

#define SHARED_START	        0x414000    // Start address of shared User memory (free to use)
#define SHARED_END		        0x500000    // End address of shared User memory (free to use)

#define SHARED_USER_CODE_START  0x500000    // Start address of shared User code
#define SHARED_USER_CODE_END	0X600000    // End address of shared User code

#define USER_CODE_START         0x600000    // Start address of User code
#define USER_CODE_END	        0X800000    // End address of User code


// Page related constants
#define PAGE_START		0x350000    // Start address of paging related memory

#define NUM_PGD         (TASK_NUMBER + 1)                                   // 1 PGD per task + Kernel
#define PGD_SIZE        0x001000                                            // Size in memory of 1 PGD
#define PGD_START       PAGE_START                                          // Start address of the first PGD (and thus all the PGDs)
#define PGD_END         (PGD_START + NUM_PGD*PGD_SIZE)                      // End address of the first PGD (and thus all the PGDs)
#define PGD_OFFSET      0x100000000                                         // Memory that can be handled by one PGD (4GB)
#define PDE_OFFSET      0x400000                                            // Address offset that provokes a change in PDE (used for mapping)

#define NUM_PTB         20                                                  // Allocated at mapping
#define PTB_SIZE        0x001000                                            // Size in memory of 1 PTB
#define PTB_START       PGD_END                                             // Start address of the first PTB (and thus all the PTBs)
#define PTB_END         (PTB_START + NUM_PTB*PTB_SIZE)                      // End address of the first PTB (and thus all the PTBs)
#define PTB_OFFSET      PDE_OFFSET                                          // Memory that can be handled by one PTB (4MB)
#define PTE_OFFSET      0x001000                                            // Address offset that provokes a change in PTE (used for mapping)

#define PAGE_END		PTB_END     // End address of paging related memory


// Segment related constants
#define GDT_SIZE    6                           // Number of segments (first one is NULL)
#define GDT_START   0x370000                    // Start address of GDT
#define GDT_LIMIT   (GDT_SIZE*8 - 1)            // Segment-relative address of last accessible byte of GDT data
#define GDT_END     (GDT_START + GDT_LIMIT + 1) // End address of GDT

#define TSS_START   0x380000                    // Start address of TSS
#define TSS_LIMIT   (sizeof(tss_t)*8 - 1)       // Segment-relative address of last accessible byte of TSS data
#define TSS_END     (TSS_START + TSS_LIMIT + 1) // End address of TSS

#endif
