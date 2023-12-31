#include <debug.h>
#include <segmem.h>
#include <cr.h>
#include <extend/print_utils.h>

void print_gdt_content(gdt_reg_t gdtr_ptr)
{
   seg_desc_t* gdt_ptr;
   gdt_ptr = (seg_desc_t*)(gdtr_ptr.addr);

   debug("\nGDT (0x%lx)\n", gdtr_ptr.addr);

   int i=0;
   while ((uint32_t)gdt_ptr < ((gdtr_ptr.addr) + gdtr_ptr.limit)) {
      uint32_t start = gdt_ptr->base_3<<24 | gdt_ptr->base_2<<16 | gdt_ptr->base_1;
      uint32_t end;

      if (gdt_ptr->g) {
         end = start + ( (gdt_ptr->limit_2<<16 | gdt_ptr->limit_1) <<12) + 4095;
      } else {
         end = start + (gdt_ptr->limit_2<<16 | gdt_ptr->limit_1);
      }

      debug("%d ", i);
      debug("[0x%x ", start);
      debug("- 0x%x] ", end);
      debug("seg_t: 0x%x ", gdt_ptr->type);
      debug("desc_t: %d ", gdt_ptr->s);
      debug("priv: %d ", gdt_ptr->dpl);
      debug("present: %d ", gdt_ptr->p);
      debug("avl: %d ", gdt_ptr->avl);
      debug("longmode: %d ", gdt_ptr->l);
      debug("default: %d ", gdt_ptr->d);
      debug("gran: %d ", gdt_ptr->g);
      debug("\n");
      gdt_ptr++;
      i++;
   }   
}

void print_gdt(void)
{
   gdt_reg_t gdtr_ptr;
   get_gdtr(gdtr_ptr);
   print_gdt_content(gdtr_ptr);
}

void print_idtr(void)
{
	idt_reg_t idtr;
	get_idtr(idtr);

	debug(
		"\nREG_IDTR :\n"
		"    limit -> %u\n"
		"    addr -> 0x%08lx\n",
		idtr.limit, idtr.addr
      );
}

void print_cr3(void)
{
	cr3_reg_t cr3;
	cr3.raw = (uint32_t)get_cr3();

	debug(
		"\nCR3 :\n"
		"	PWT 	-> %d\n"
		"	PCD 	-> %d\n"
		"	PD Base -> 0x%08x\n\n",
		cr3.pwt, cr3.pcd, cr3.addr<<12
      );
}
