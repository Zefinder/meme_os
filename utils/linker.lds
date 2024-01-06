/* GPLv2 (c) Airbus */
OUTPUT_FORMAT("elf32-i386","elf32-i386","elf32-i386");
OUTPUT_ARCH("i386")

ENTRY(entry)

PHDRS
{
   phboot  PT_LOAD FLAGS (7);
   phstack PT_LOAD FLAGS (6);
   phsetup PT_LOAD FLAGS (7);
}

SECTIONS
{
   . = 0x300000;
   .mbh      : { KEEP(*(.mbh)) . = ALIGN(4);     } : phboot
   .stack    : { KEEP(*(.stack))                 } : phstack

   __kernel_start__ = .;

   .idt_jmp  : { KEEP(*(.idt_jmp))               } : phsetup
   .text     : { *(EXCLUDE_FILE(task1* task2*) .text)    } : phsetup
   .rodata   : { *(.rodata)                      } : phsetup
   .data     : { *(.data)                        } : phsetup
   .bss      : { *(.bss COMMON)                  } : phsetup
   /DISCARD/ : { *(.note* .indent .comment)      } : phsetup

   __kernel_end__ = .;

   . = 0x500000;

   __user_shared_code_start__ = .;

   .shared_usr_code : { *(.text)              } : phsetup

   . = 0x600000;

   __user_code_start__ = .;

   .task1 : { task1*(.text)                 } : phsetup

   . = 0x601000;

   /*. = ALIGN(4096); to use when we have input files named task1*/

   .task2 : { task2*(.text)                 } : phsetup

   . = 0x602000;
   
   /*. = ALIGN(4096); to use when we have input files named task2*/

   __user_code_end__ = .;

   /*.usr      : { *(.usr)                         } : phsetup*/
}
