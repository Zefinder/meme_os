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
            debug("SYS_READ_CNT\n");
            offset_t virt = ctx->gpr.ebx.raw;
            debug("virt ->%p\n", (void*)virt);

            uint32_t tidx = current_task();

            pde32_t* task_PGD = nth_user_pgds(tidx);
            debug("task_PGD ->%p\n", task_PGD);
            pte32_t* task_PTB = (pte32_t*)((task_PGD[pd32_idx(virt)].addr) << 12);
            debug("task_PTB ->%p\n", task_PTB);
            offset_t phys = ( (task_PTB[pt32_idx(virt)].addr) << 12 ) | ( (virt & ((1u << 10) - 1)) );
            debug("phys ->%p\n", (void*)phys);

            // Trick for compiling
            // TODO
            uint32_t a;
            a = (uint32_t)phys;
            a = (uint32_t)tidx;
            a = (uint32_t)task_PGD;
            a = (uint32_t)task_PTB;
            a = a;

            break;
        
        default:
            break;

    }
    debug("Exiting SYSCALL\n");
}


void init_idt(void)
{
	idt_reg_t idtr;
	get_idtr(idtr);

    // Setup syscall via interrupt
	idtr.desc[INT_SYSCALL].offset_1 = (uint16_t)((uint32_t)syscall_isr & 0xffff);
	idtr.desc[INT_SYSCALL].offset_2 = (uint16_t)((uint32_t)syscall_isr >> 16);
    idtr.desc[INT_SYSCALL].dpl = 3;
}
