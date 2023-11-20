/* GPLv2 (c) Airbus */
/* Made by Adrien Jakubiak and Nicolas Rigal */
/* TLS-SEC 2023-2024 */
/* Meme is everywhere, don't forget to laugh */
#include <debug.h>
#include <segmem.h>
#include <pagemem.h>
#include <intr.h>
#include <string.h>
#include <cr.h>
#include "printutils.h"

// Defines for priviliges
#define KERNEL_PRIVILEGE 0
#define USER_PRIVILEGE 3

// Defines for segment descriptors
#define SEG_DESC_TYPE_SYSTEM 0
#define SEG_DESC_TYPE_DATACODE 1
#define SEG_DESC_NO_GRANULAR 0
#define SEG_DESC_GRANULAR 1

// Gate types
#define TASK_GATE 0b0101
#define INTERRUPT_GATE_16 0b0110
#define TRAP_GATE_16 0b0111
#define INTERRUPT_GATE_32 0x1110
#define TRAP_GATE_32 0b1111

// Interruptions to catch
#define SYSCALL_INTERRUPTION 0x80

// Constants
#define CS0 0x0008
#define CS3 0x001B
#define DS0 0x0010
#define DS3 0x0023
#define TSS 0x0028
#define SEGMENT_NUMBER 6
#define USER_ADDRESS 0x300000

// Null segment
#define null_segment          \
   ({                         \
      seg_desc_t segment;     \
      segment.raw = 0;        \
      segment;                \
   })                

// Paging macros
#define enable_paging()						\
	{set_cr0(get_cr0() | (1 << 31) | 1);}

#define cr3_config(_addr_)							\
	{set_cr3(get_cr3() | (_addr_ >> 12) << 12);}

#define pde_set(_pde_,_vaddr_,_pteaddr_)												\
	({																					\
		pg_zero(&(_pde_)[pd32_idx(_vaddr_)]);											\
		pg_set_entry(&(_pde_)[pd32_idx(_vaddr_)], PG_RW, pg_4K_nr((int) _pteaddr_));	\
	})

#define pte_set(_pte_,_vaddr_,_paddr_)											\
	({																			\
		pg_zero(&(_pte_)[pt32_idx(_vaddr_)]);									\
		pg_set_entry(&(_pte_)[pt32_idx(_vaddr_)], PG_RW, pg_4K_nr(_paddr_));	\
	})

#define pte_identity_set(_pte_,_addr_)	\
	({									\
		pte_set(_pte_,_addr_,_addr_);	\
	})


// Global variables
gdt_reg_t gdtr_ptr;
idt_reg_t idtr_ptr;
seg_desc_t gdt_ptr[SEGMENT_NUMBER];
tss_t TSS_entry;

pde32_t* pgd_entry = (pde32_t*) 0x600000;
pte32_t* pte_entry = (pte32_t*) 0x601000;
// Other PTE here

void userland() {
	debug("Userland!\n");
	// uint32_t arg = 0x2023;
	// asm volatile ("int $%0"::"i"(SYSCALL_INTERRUPTION), "a"(0x69), "S"(syscall_string));
	// asm volatile ("int $48"::"a"(0x420), "S"(0x2000));
	while(1);
}

/* ----------------------------------------------- */
/*                Segment functions                */
/* ----------------------------------------------- */

seg_desc_t create_segment(int base, int limit, int type, int s, int dpl, int g) {
	seg_desc_t segment;
	segment.limit_1 = limit & 0xFFFF;
	segment.base_1 = base & 0xFFFF;
	segment.base_2 = (base >> 16) & 0xFF;
	segment.type = type;
	segment.s = s;
	segment.dpl = dpl;
	segment.p = 0b1;           // Present because we will use it
	segment.limit_2 = (limit >> 16) & 0xF;
	segment.avl = 0b1;         // Wtf ?
	segment.l = 0b0;           // We don't want a 64 bit segment
	segment.d = 0b1;           // 32 bit
	segment.g = g;             // Granular ?
	segment.base_3 = (base >> 24) & 0xFF;

	return segment;
}

seg_desc_t create_flat_segment(int type, int s, int dpl) {
	return create_segment(0x00000000, 0xFFFFF, type, s, dpl, SEG_DESC_GRANULAR);
}

/* ----------------------------------------------- */
/*             Interruption functions              */
/* ----------------------------------------------- */

int_desc_t create_int_desc(int dpl, int gate, int handler_address) {
	int_desc_t intr_seg;
	intr_seg.offset_1 = handler_address & 0xFFFF;		   	// Least part of handler
	intr_seg.selector = CS0;								// Mmmmh
	intr_seg.ist = 0b000;									// Nothing, for 64 bits! 
	intr_seg.zero_1 = 0b00000;								// ZERO
	intr_seg.type = gate;									// Trap gate 
	intr_seg.zero_2 = 0b0;									// ZEROOO
	intr_seg.dpl = dpl;										// Kernel or user
	intr_seg.p = 0b1;										// Present!
	intr_seg.offset_2 = (handler_address >> 16) & 0xFFFF;   // High part of handler

	return intr_seg;
}

void set_int_desc(int dpl, int gate, int handler_address, int index) {
	int_desc_t intr_seg = create_int_desc(dpl, gate, handler_address);

	int_desc_t* intr_segments = (int_desc_t*) idtr_ptr.addr;
	intr_segments[index] = intr_seg;
}

/* ----------------------------------------------- */
/*              Interruption handlers              */
/* ----------------------------------------------- */

void syscall_isr() {
	asm volatile (
		"leave\n\t"
		"pusha\n\t"
		"mov %esp, %eax\n\t"
		"call syscall_handler\n\t"
		"popa\n\t"
		"iret\n\t"
	);
}

void __regparm__(1) syscall_handler(int_ctx_t *ctx) {
	debug("SYSCALL eax = 0x%x\n", ctx->gpr.eax.raw);
	int esi = ctx->gpr.esi.raw;
	if (esi < USER_ADDRESS) {
		debug("Laisser fuiter de la donnée du noyau moi ? serieusement ^^ haha on me l avait pas sortie celle la depuis loooongtemps :)\n");
	} else {
		debug("%s\n", (char*) ctx->gpr.esi.raw);
	}
}

/* ----------------------------------------------- */
/*              Switch to user mode !              */
/* ----------------------------------------------- */

void switch_to_user_mode() {
	// Simulating a return from interruption
	asm volatile(
		"pushl %0\n\t"
		"pushl %%ebp\n\t"
		"pushf\n\t"
		"pushl %1\n\t"
		"pushl %2\n\t"
		"iret\n\t"
		:
		: "i" (DS3), "i" (CS3), "r" ((int) userland)
	);
}

/* ----------------------------------------------- */
/*                  Init functions                 */
/* ----------------------------------------------- */

void init_gdt() {
	get_gdtr(gdtr_ptr);

	// First segment always NULL
	gdt_ptr[0] = null_segment;

	// Kernel code and data segments (flat)
	gdt_ptr[1] = create_flat_segment(SEG_DESC_CODE_XR, SEG_DESC_TYPE_DATACODE, KERNEL_PRIVILEGE);
	gdt_ptr[2] = create_flat_segment(SEG_DESC_DATA_RW, SEG_DESC_TYPE_DATACODE, KERNEL_PRIVILEGE);

	// User code and data segments (flat)
	gdt_ptr[3] = create_flat_segment(SEG_DESC_CODE_XR, SEG_DESC_TYPE_DATACODE, USER_PRIVILEGE);
	gdt_ptr[4] = create_flat_segment(SEG_DESC_DATA_RW, SEG_DESC_TYPE_DATACODE, USER_PRIVILEGE);

	// TSS segment (system)
	gdt_ptr[5] = create_segment((offset_t) &TSS_entry, sizeof(tss_t), SEG_DESC_SYS_TSS_AVL_32,
									SEG_DESC_TYPE_SYSTEM, KERNEL_PRIVILEGE, SEG_DESC_NO_GRANULAR);

	gdtr_ptr.addr = (int) gdt_ptr;
	gdtr_ptr.desc = gdt_ptr;
	gdtr_ptr.limit = 8 * SEGMENT_NUMBER - 1;

	debug("Setting GDTR...\n");
	set_gdtr(gdtr_ptr);
	debug("GDTR set!\n");

	print_gdt_content(gdtr_ptr);
	debug("\n");
}

void init_TSS(uint32_t esp0) {
	// Changing selectors to ring 3
	set_ds(DS3);
	set_es(DS3);
	set_fs(DS3);
	set_gs(DS3);

	// Setting struct to 0
	memset(&TSS_entry, 0, sizeof(TSS_entry));

	// Putting TSS at ring 0 but with user privilege so users can use it
	TSS_entry.s0.esp = esp0;
	TSS_entry.s0.ss = DS0;
	TSS_entry.cs = CS0 | USER_PRIVILEGE;
	TSS_entry.ss = DS0 | USER_PRIVILEGE;
	TSS_entry.ds = DS0 | USER_PRIVILEGE;
	TSS_entry.es = DS0 | USER_PRIVILEGE;
	TSS_entry.fs = DS0 | USER_PRIVILEGE;
	TSS_entry.gs = DS0 | USER_PRIVILEGE;

	// Updating TSS
	debug("Setting TSS...\n");
	set_tr(TSS);
	debug("TSS set!\n");
}

void init_idt() {
	debug("Setting IDT...\n");
	get_idtr(idtr_ptr);
	
	// Catching syscall interruption
	set_int_desc(USER_PRIVILEGE, TRAP_GATE_32, (int) &syscall_isr, SYSCALL_INTERRUPTION);
	debug("IDT set!\n");
}

void init_paging() {

}

/* ----------------------------------------------- */
/*                  MAIN ENTRANCE                  */
/* ----------------------------------------------- */

void tp() {
	// Init GDT
	init_gdt();

	// Init IDT
	init_idt();

	// Init TSS segment
	init_TSS(get_ebp());

	// Init paging
	init_paging();

	// User mode !!!!
	switch_to_user_mode();
}