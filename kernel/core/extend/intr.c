#include <segmem.h>
#include <intr.h>
#include <extend/memory.h>
#include <extend/intr.h>
#include <extend/pagemem.h>
#include <debug.h>


void syscall_isr() {
   asm volatile (
      "leave ; pusha\n\t"
      "mov %esp, %eax\n\t"
      "call syscall_handler\n\t"
      "popa ; iret\n\t"
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
        case SYS_READ_CNT:
            asm volatile ("mov %eax, %cr0\n\t");
            debug("YEEEEEHAW RING 0 BABYYY");
            
            offset_t cnt = ctx->gpr.eax.raw;
            uint32_t tidx = current_task();

            pde32_t* task_PGD = nth_user_pgds(tidx);
            pte32_t* task_PTB = nth_user_ptbs(tidx);

            // Trick for compiling
            // TODO
            cnt = cnt;
            tidx = tidx;
            task_PGD = task_PGD;
            task_PTB = task_PTB;

            break;
        
        default:
            break;

    }
}


void init_idt(void)
{
	idt_reg_t idtr;
	get_idtr(idtr);

    // Setup INT 48 as syscall via interrupt
	idtr.desc[48].offset_1 = (uint16_t)((uint32_t)syscall_isr & 0xffff);
	idtr.desc[48].offset_2 = (uint16_t)((uint32_t)syscall_isr >> 16);
    idtr.desc[48].dpl = 3;
}