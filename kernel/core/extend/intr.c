#include <segmem.h>
#include <intr.h>
#include <io.h>
#include <asm.h>
#include <debug.h>
#include <string.h>

#include <extend/memory.h>
#include <extend/intr.h>
#include <extend/syscall.h>
#include <extend/pagemem.h>
#include <extend/task_manager.h>
#include <extend/timer.h>

void syscall_isr() {
   asm volatile (
      "leave\n\t""pusha\n\t"
      "mov %esp, %eax\n\t"
      "call syscall_handler\n\t"
      "popa\n\t""iret\n\t"
      );
}

void __regparm__(1) syscall_handler(int_ctx_t *ctx) {
    debug("\n-= SYSCALL eax = %08x =-\n", ctx->gpr.eax.raw);
    switch(ctx->gpr.eax.raw) // Which syscall is it ?
    {
        /*
            Read counter at address in ebx.
            ebx is in task's address space -> need to translate to kernel address space
        */
        case READ_SHARED_SYSCALL:
            offset_t virt, phys;

            debug("READ_SHARED_SYSCALL:\n");
            virt = ctx->gpr.ebx.raw;
            debug("\tvirt     -> %p\n", (void*)virt);

            // uint32_t tidx = current_task();

            // pde32_t* task_PGD = nth_user_pgds(tidx);
            pde32_t* task_PGD = nth_pgd_gbl(0);
            debug("\ttask_PGD -> %p\n", task_PGD);
            
            pte32_t* task_PTB = (pte32_t*)((task_PGD[pd32_idx(virt)].addr) << 12);
            debug("\ttask_PTB -> %p\n", task_PTB);

            phys = ( (task_PTB[pt32_idx(virt)].addr) << 12 ) | ( (virt & ((1u << 12) - 1)) );
            debug("\tphys     -> %p\n", (void*)phys);

            debug("\n\tCNT = %d\n", *((int*)phys));

            ctx->gpr.edx.raw = *((int*) phys);

            break;
        
        default:
            break;

    }
    debug("-= Exiting SYSCALL =-\n");
}

void irq0_isr() {
   asm volatile (
      "leave\n\t" "pusha\n\t"
      "mov %esp, %eax\n\t"
      "call irq0_handler\n\t"
      "popa\n\t" "iret\n\t"
      );
}

// int tick = 0;
// This calls the scheduler
void __regparm__(1) irq0_handler(int_ctx_t *ctx) {
    debug("\n-= IRQ0 ebx = %08x =-\n", ctx->gpr.ebx.raw);
    tidx old_task = current_task();

    // if (++tick % 18 == 0)
        debug("Switching tasks!\n");
        int_ctx_t *new_ctx = schedule();
        debug("New context retreived\n");

        // If we switched tasks, save old task's context, load new task's context in stack for popa; iret
        // if (current_task() != old_task) {
            debug("Saving context\n");
            save_task_ctx(old_task, ctx);
            debug("Context saved, loading new context\n");
            memcpy(new_ctx, ctx, sizeof(int_ctx_t));
            debug("New context loaded, loading new PGD\n");

            pde32_t *task_PGD = nth_user_pgds(current_task());
            cr3_reg_t cr3;
            cr3_pgd_set(&cr3, &task_PGD[0]);
            set_cr3(cr3);
            debug("PGD loaded\n");
        // } else {
        //     debug("Same task, continuing\n");
        // }
}

    increment_timer();
}

void set_interval() {
    out(PIT_COMMAND_REGISTER, PIT_CHANNEL_0_SEL | PIT_ACCESS_MODE_BOTH | PIT_OPERATING_MODE_3 | PIT_BINARY_MODE);
}

void init_idt(void)
{
	idt_reg_t idtr;
	get_idtr(idtr);

    // Setup IRQ0 ISR
    set_interval();
	idtr.desc[INT_IRQ0].offset_1 = (uint16_t)((uint32_t)irq0_isr & 0xffff);
	idtr.desc[INT_IRQ0].offset_2 = (uint16_t)((uint32_t)irq0_isr >> 16);
    idtr.desc[INT_IRQ0].dpl = SEG_SEL_KRN;

    // Setup syscall via interrupt
	idtr.desc[INT_SYSCALL].offset_1 = (uint16_t)((uint32_t)syscall_isr & 0xffff);
	idtr.desc[INT_SYSCALL].offset_2 = (uint16_t)((uint32_t)syscall_isr >> 16);
    idtr.desc[INT_SYSCALL].dpl = SEG_SEL_USR;
    
    int interrupt_mask = in(PIC_MASTER_PORT); // Reading PIC master port
    out(PIC_MASTER_PORT, interrupt_mask & IRQ0_MASK_ENABLE); // Enabling IRQ0 in PIC
    force_interrupts_on();
}
