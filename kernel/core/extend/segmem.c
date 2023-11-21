#include <extend/segmem.h>
#include <extend/memory.h>
#include <extend/print_utils.h>

void init_gdt()
{
   seg_desc_t* GDT = (seg_desc_t*)GDT_START;
   tss_t*      TSS = (tss_t*)TSS_START;

   gdt_reg_t gdtr;
   gdtr.desc = &GDT[0];
   gdtr.limit = GDT_LIMIT;

   // First segment always NULL
   GDT[0].raw = 0ull;


   // Set kernel code and data segments (flat)
   set_c0_dsc(&GDT[c0_idx]);
   set_d0_dsc(&GDT[d0_idx]);

   // Set user code and data segments (flat)
   set_c3_dsc(&GDT[c3_idx]);
   set_d3_dsc(&GDT[d3_idx]);

   // Set TSS segment (system)
   gdt_set_tss_dsc(&GDT[ts_idx], (offset_t)TSS);


   // Load into register
   set_gdtr(gdtr);


   // Initialize code and data segments to kernel segments
   set_cs(c0_sel);

   set_ss(d0_sel);
   set_ds(d0_sel);
   set_es(d0_sel);
   set_fs(d0_sel);
   set_gs(d0_sel);
}