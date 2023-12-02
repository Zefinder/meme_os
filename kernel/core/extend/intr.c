#include <memory.h>
#include <segmem.h>
#include <intr.h>
#include <extend/intr.h>

#define get_pid()   1

void syscall_isr() {
   asm volatile (
      "leave ; pusha        \n"
      "mov %esp, %eax      \n"
      "call syscall_handler \n"
      "popa ; iret"
      );
}

void __regparm__(1) syscall_handler(int_ctx_t *ctx) {
    debug("SYSCALL eax = %08x\n", ctx->gpr.eax.raw);

    switch(ctx->gpr.eax.raw) // Which syscall is it ?
    {
        /*
            Read counter at address in ebx.
            ebx is in task's address space -> need to translate to kernel address space
        */
        case 1:
            offset_t cnt = ctx->gpr.eax.raw;
            int pid = get_pid();
            
            // pde32_t* PGD = nth_pgd_gbl(0);
            // pte32_t* PTB = nth_ptb_gbl(0);

            pde32_t* task_PGD = nth_pgd_gbl(0);
            pte32_t* task_PTB = nth_ptb_gbl(0);

            break;
        
        default:
            break;

    }

   // Q4
   char* s = (char*)get_esi();
   if ((offset_t)s >= (offset_t)USER_START)
      debug("ESI points to: %s\n\n", s);
}


void init_idt(void)
{
	idt_reg_t idtr;
	get_idtr(idtr);
	debug(
		"\nREG_IDTR :\n"
		"    limit -> %u\n"
		"    addr -> 0x%08lx\n",
		idtr.limit,
		idtr.addr
	);

    // Setup INT 48 as syscall via interrupt
	idtr.desc[48].offset_1 = (uint16_t)((uint32_t)syscall_isr & 0xffff);
	idtr.desc[48].offset_2 = (uint16_t)((uint32_t)syscall_isr >> 16);
}
