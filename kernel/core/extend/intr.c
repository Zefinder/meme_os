#include <segmem.h>
#include <intr.h>
#include <io.h>
#include <asm.h>
#include <debug.h>

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
    debug("SYSCALL eax = %08x\n", ctx->gpr.eax.raw);
    switch(ctx->gpr.eax.raw) // Which syscall is it ?
    {
        /*
            Read counter at address in ebx.
            ebx is in task's address space -> need to translate to kernel address space
        */
        case READ_SHARED_SYSCALL:
            debug("READ_SHARED_SYSCALL\n");
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

            ctx->gpr.edx.raw = *((int*) phys);

            break;
        
        default:
            break;

    }
    debug("Exiting SYSCALL\n");
}

void irq0_isr() {
   asm volatile (
      "leave\n\t" "pusha\n\t"
      "mov %esp, %eax\n\t"
      "call irq0_handler\n\t"
      "movb $0x20, %al\n\t" // Set to PIC control port
      "movw $0x20, %dx\n\t" // Send EOI to PIC
      "outb %al, %dx\n\t" 
      "popa\n\t" "iret\n\t"
      );
}

int tick = 0;
void __regparm__(1) irq0_handler(int_ctx_t *ctx) {
    ctx->gpr.ebx.raw =ctx->gpr.ebx.raw;
    if (++tick % IRQ0_WAITING_TICKS == 0) {
        // Call scheduler
        // debug("Switching tasks!\n");

        // Reset ticks
        tick -= IRQ0_WAITING_TICKS;
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
