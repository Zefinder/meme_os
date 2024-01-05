#include <segmem.h>
#include <intr.h>
#include <io.h>
#include <extend/memory.h>
#include <extend/intr.h>
#include <extend/pagemem.h>
#include <extend/task_manager.h>
#include <debug.h>


void syscall_isr() {
   asm volatile (
      "leave ; pusha\n\t"
      "mov %esp, %eax\n\t"
      "call syscall_handler\n\t"
      "popa\n\t ; iret\n\t"
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
        case SYS_READ_CNT:
            offset_t virt, phys;

            debug("SYS_READ_CNT:\n");
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
// This the scheduler
void __regparm__(1) irq0_handler(int_ctx_t *ctx) {
    debug("IRQ0 ebx = %08x\n", ctx->gpr.ebx.raw);
    // if (++tick % 18 == 0)
        debug("Switching tasks!\n");
}

// void set_interval()
// {
//   int divisor = IRQ0_FREQUENCY; // divisor = 1193180/1000
//   outb(PIT_COMMAND_REGISTER, PIT_CHANNEL_0_SEL | PIT_ACCESS_MODE_BOTH | PIT_OPERATING_MODE_3 | PIT_BINARY_MODE); // outb(0x43, 0x36)
//   outb(PIT_CHANNEL_0_PORT, divisor & 0xFF); // PIT_CHANNEL_0_PORT = 0x40
//   outb(PIT_CHANNEL_0_PORT, divisor >> 8);
// }

void init_idt(void)
{
	idt_reg_t idtr;
	get_idtr(idtr);

    // set_interval();
    // // Setup syscall via interrupt
	// idtr.desc[INT_IRQ0].offset_1 = (uint16_t)((uint32_t)irq0_isr & 0xffff);
	// idtr.desc[INT_IRQ0].offset_2 = (uint16_t)((uint32_t)irq0_isr >> 16);
    // idtr.desc[INT_IRQ0].dpl = 3; // Test appel direct

    // Setup syscall via interrupt
	idtr.desc[INT_SYSCALL].offset_1 = (uint16_t)((uint32_t)syscall_isr & 0xffff);
	idtr.desc[INT_SYSCALL].offset_2 = (uint16_t)((uint32_t)syscall_isr >> 16);
    idtr.desc[INT_SYSCALL].dpl = 3;
}
