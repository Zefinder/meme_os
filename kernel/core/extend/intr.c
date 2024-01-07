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

int schedule_enabled = 0;

void syscall_isr() {
   asm volatile (
      "leave\n\t""pusha\n\t"
      "mov %esp, %eax\n\t"
      "call syscall_handler\n\t"
      "popa\n\t""iret\n\t"
      );
}

int translate_address(int address) {
    // Address translation
    offset_t phys;
    // uint32_t tidx = current_task();
    // pde32_t* task_PGD = nth_user_pgds(tidx);
    // debug("\tvirt     -> %p\n", (void*)address);

    pde32_t* task_PGD = nth_pgd_gbl(0);
    // debug("\ttask_PGD -> %p\n", task_PGD);
    
    pte32_t* task_PTB = (pte32_t*)((task_PGD[pd32_idx(address)].addr) << 12);
    // debug("\ttask_PTB -> %p\n", task_PTB);
    
    phys = ( (task_PTB[pt32_idx(address)].addr) << 12 ) | ( (address & ((1u << 12) - 1)) );
    // debug("\tphys     -> %p\n", (void*)phys);

    return phys;
}

void __regparm__(1) syscall_handler(int_ctx_t *ctx) {
    // debug("\n-= SYSCALL eax = %08x =-\n", ctx->gpr.eax.raw);

    // Which syscall is it ?
    switch(ctx->gpr.eax.raw) 
    {
        /*
            Read counter at address in ebx.
            ebx -> address of the task (to translate)
            edx -> return creation value
        */
        case CREATE_NEW_TASK_SYSCALL:
            // debug("CREATE_NEW_TASK_SYSCALL:\n");
            // TODO A faire ssi pas dans cr3 kernel
            // int task_address = translate_address(ctx->gpr.ebx.raw);
            int task_address = ctx->gpr.ebx.raw;
            ctx->gpr.edx.raw = create_task((void*) task_address);
            break;
        
        /*
            Kills current task. This is automatically called when a 
            task hits return.
        */
        case KILL_TASK_SYSCALL:
            // debug("KILL_TASK_SYSCALL:\n");
            tidx current_index = current_task();
            ctx->gpr.edx.raw = end_task(current_index);
            break;

        /*
            Prints to stdout the char* and the value
            ebx -> format (to translate)
            ecx -> value
        */
        case WRITE_STDOUT_SYSCALL:
            // debug("WRITE_STDOUT_SYSCALL:\n");
            int format_address = translate_address(ctx->gpr.ebx.raw);
            int params_address = translate_address(ctx->gpr.ecx.raw);
            char* format = (char*) format_address;
            va_list params = *(va_list*) params_address;
            __vprintf(format, params);
            break;

        /*
            Read counter at address in ebx.
            ebx -> address in shared page (to translate)
            edx -> return value
        */
        case READ_SHARED_SYSCALL:
            // debug("READ_SHARED_SYSCALL:\n");
            int shared_page_address_read = translate_address(ctx->gpr.ebx.raw);
            ctx->gpr.edx.raw = *((int*) shared_page_address_read);
            break;

        /*
            Read counter at address in ebx.
            ebx -> address in shared page (to translate)
            ecx -> value to write
        */
        case WRITE_SHARED_SYSCALL:
            // debug("WRITE_SHARED_SYSCALL:\n");
            int shared_page_address_write = translate_address(ctx->gpr.ebx.raw);
            *((int*) shared_page_address_write) = ctx->gpr.ecx.raw;
            break;

        /*
            Prints tasks in the running task array
        */
        case SHOW_RUNNING_TASKS:
            // debug("SHOW_RUNNING_TASKS:\n");
            struct task_t* tasks = show_tasks();
            debug(" PID\tFIRST PAGE\tHAS SECOND PAGE\tQUANTUM\tIS ALIVE\n");
            for (tidx task_index = 0; task_index < TASK_NUMBER; task_index++) {
                struct task_t task = tasks[task_index];
                char* yes = "YES";
                char* no = "NO";

                debug(" %d\t0x%08lx\t%s\t\t%d\t%s\n",
                    task.task_id,
                    task.first_page_address,
                    task.has_second_page ? yes : no,
                    task.quantum,
                    task.is_alive ? yes : no);
            }
            break;  

        case START_SCHEDULING_SYSCALL:
            // debug("START_SCHEDULING_SYSCALL:\n");
            schedule_enabled = 1;
            break;

        case SHUTDOWN_SYSCALL:
            debug("%s", "\nShuting down memeOS, we hope that you had a pleaseant flight and wish you a good day!\n");
            while(1) halt();
            break;

        default:
            break;
    }
    // debug("-= Exiting SYSCALL =-\n");
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
// This calls the scheduler
void __regparm__(1) irq0_handler(int_ctx_t *ctx) {
    // tidx old_task = current_task();

    if (schedule_enabled) {
        if (++tick % IRQ0_WAITING_TICKS == 0) {
            debug("\n-= IRQ0 ebx = %08x =-\n", ctx->gpr.ebx.raw);
            debug("Switching tasks!\n");
            // int_ctx_t *new_ctx = schedule();
            debug("New context retreived\n");

            // If we switched tasks, save old task's context, load new task's context in stack for popa; iret
            // if (current_task() != old_task) {
            debug("Saving context\n");
            // save_task_ctx(old_task, ctx);
            debug("Context saved, loading new context\n");
            // memcpy(new_ctx, ctx, sizeof(int_ctx_t));
            debug("New context loaded, loading new PGD\n");

            // pde32_t *task_PGD = nth_user_pgds(current_task());
            // cr3_reg_t cr3;
            // cr3_pgd_set(&cr3, &task_PGD[0]);
            // set_cr3(cr3);
            // debug("PGD loaded\n");
            // } else {
            //     debug("Same task, continuing\n");
        }
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
