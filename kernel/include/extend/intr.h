#ifndef __EXTEND_INTR_H__
#define __EXTEND_INTR_H__

#include <intr.h>

// PIC and PIT
#define PIC_MASTER_PORT         0x21
#define PIT_CHANNEL_0_PORT      0x40
#define PIT_COMMAND_REGISTER    0x43

#define PIT_CHANNEL_0_SEL       0b00000000
#define PIT_CHANNEL_1_SEL       0b01000000
#define PIT_CHANNEL_2_SEL       0b10000000

#define PIT_ACCESS_MODE_LATCH   0b00000000
#define PIT_ACCESS_MODE_LOW     0b00010000
#define PIT_ACCESS_MODE_HIGH    0b00100000
#define PIT_ACCESS_MODE_BOTH    0b00110000

#define PIT_OPERATING_MODE_0    0b00000000 // Interrupt
#define PIT_OPERATING_MODE_1    0b00000010 // Hardware retriggable one-shot
#define PIT_OPERATING_MODE_2    0b00000100 // Rate generator
#define PIT_OPERATING_MODE_3    0b00000110 // Software triggered
#define PIT_OPERATING_MODE_4    0b00001000 // Hardware triggered
#define PIT_OPERATING_MODE_5    0b00001010 // Squarewave generator

#define PIT_BINARY_MODE         0b00000000
#define PIT_BCD_MODE            0b00000001

#define IRQ0_MASK_ENABLE        0b11111110

// Interrupts
#define INT_IRQ0        0x20
#define INT_SYSCALL     0x80

// IRQ constants
#define PIT_CHANNEL0_FREQUENCY  18  // Hz
#define IRQ0_WAITING_TICKS      18  // Divides frequency (18 is 1 per second)

void init_idt(void);
void __regparm__(1) __attribute__((section(".shared_usr_code"),aligned(4))) irq0_handler(int_ctx_t *ctx);

#endif
