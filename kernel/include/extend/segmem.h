#ifndef SEGMEM_H
#define SEGMEM_H

#include <segmem.h>

/** Segment descriptor indexes **/
#define c0_idx    1
#define d0_idx    2
#define c3_idx    3
#define d3_idx    4
#define ts_idx    5

/** Return corresponding segment selector value **/
#define c0_sel    gdt_krn_seg_sel(c0_idx)
#define d0_sel    gdt_krn_seg_sel(d0_idx)
#define c3_sel    gdt_usr_seg_sel(c3_idx)
#define d3_sel    gdt_usr_seg_sel(d3_idx)
#define ts_sel    gdt_krn_seg_sel(ts_idx)


/***************************************************/
/**    Setups flat descriptor                     **/
/**                                               **/
/**    _dsc_ : address of descriptor to setup     **/
/**    _pvl_ : wanted privilege for descriptor    **/
/**    _typ_ : wanted descriptor type             **/
/***************************************************/
#define gdt_set_flat_dsc(_dsc_,_pvl_,_typ_)     \
   {                                            \
      (_dsc_)->limit_1   = 0xffff;              \
      (_dsc_)->base_1    = 0x0000;              \
      (_dsc_)->base_2    = 0x00;                \
      (_dsc_)->type      = _typ_;               \
      (_dsc_)->s         = 1;                   \
      (_dsc_)->dpl       = _pvl_;               \
      (_dsc_)->p         = 1;                   \
      (_dsc_)->limit_2   = 0xf;                 \
      (_dsc_)->avl       = 1;                   \
      (_dsc_)->l         = 0;                   \
      (_dsc_)->d         = 1;                   \
      (_dsc_)->g         = 1;                   \
      (_dsc_)->base_3    = 0x0;                 \
   }


/*************************************************************/
/**    Setups TSS descriptor                                **/
/**                                                         **/
/**    _dsc_ : address of descriptor to setup               **/
/**    _tss_ : address of TSS to reference in descriptor    **/
/*************************************************************/
#define gdt_set_tss_dsc(_dsc_,_tss_)                     \
   {                                                     \
      raw32_t addr = {.raw = _tss_};                     \
      (_dsc_)->limit_1   = TSS_LIMIT;                    \
      (_dsc_)->base_1    = addr.wlow;                    \
      (_dsc_)->base_2    = addr._whigh.blow;             \
      (_dsc_)->type      = SEG_DESC_SYS_TSS_AVL_32;      \
      (_dsc_)->s         = 0;                            \
      (_dsc_)->dpl       = 0;                            \
      (_dsc_)->p         = 1;                            \
      (_dsc_)->limit_2   = 0x0;                          \
      (_dsc_)->avl       = 0;                            \
      (_dsc_)->l         = 0;                            \
      (_dsc_)->d         = 0;                            \
      (_dsc_)->g         = 0;                            \
      (_dsc_)->base_3    = addr._whigh.bhigh;            \
   }


/****************************************************/
/**    Setups corresponding segment descriptor     **/
/**                                                **/
/**     _dsc_ : address of descriptor so setup     **/
/****************************************************/
#define set_c0_dsc(_dsc_)      gdt_set_flat_dsc(_dsc_,0,SEG_DESC_CODE_XR)
#define set_d0_dsc(_dsc_)      gdt_set_flat_dsc(_dsc_,0,SEG_DESC_DATA_RW)
#define set_c3_dsc(_dsc_)      gdt_set_flat_dsc(_dsc_,3,SEG_DESC_CODE_XR)
#define set_d3_dsc(_dsc_)      gdt_set_flat_dsc(_dsc_,3,SEG_DESC_DATA_RW)


/******************************************************/
/**    Setups corresponding segment descriptor at    **/
/**    corresponding descriptor address              **/
/**                                                  **/
/**    _gdt_ : address of main gdt                   **/
/******************************************************/
#define gdt_set_c0_dsc(_gdt_)      set_c0_dsc(&(_gdt_)[c0_idx])
#define gdt_set_d0_dsc(_gdt_)      set_d0_dsc(&(_gdt_)[d0_idx])
#define gdt_set_c3_dsc(_gdt_)      set_c3_dsc(&(_gdt_)[c3_idx])
#define gdt_set_d3_dsc(_gdt_)      set_d3_dsc(&(_gdt_)[d3_idx])


void init_gdt(void);

#endif