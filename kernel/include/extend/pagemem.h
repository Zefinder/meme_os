#ifndef PAGEMEM_H
#define PAGEMEM_H

#include <cr.h>
#include <pagemem.h>


#define cr3_pgd_set(_cr3_,_pgd_)		\
	{(_cr3_)->raw = (offset_t)_pgd_;}

#define cr0_pg_en(_cr0_)				\
	{(_cr0_)->raw |= CR0_PG;}

#define pgd_pde(_pde_,_ptb_)					\
	{											\
		(_pde_)->raw 	= (offset_t)_ptb_;		\
		(_pde_)->p		= 1;					\
		(_pde_)->rw		= 1;					\
		(_pde_)->lvl	= 0;					\
		(_pde_)->pwt	= 0;					\
		(_pde_)->pcd	= 0;					\
		(_pde_)->acc	= 0;					\
		(_pde_)->mbz	= 0;					\
		(_pde_)->avl	= 0;					\
	}

#define ptb_pte(_pte_,_phys_)						\
	pg_set_entry(_pte_, PG_RW, pg_4K_nr(_phys_))


/*************************************************/
/**    Initializes PDE with identity mapping    **/
/**                                             **/
/**    PDE entries are aligned with 0x1000      **/
/**                                             **/
/**    _pgd_ : address of PGD                   **/
/**    _ptb_ : address of PTB                   **/
/**    _virt_ : virtual address to map          **/
/*************************************************/
#define pgd_identity_pde(_pgd_,_ptb_,_virt_)							\
	pgd_pde( &(_pgd_)[pd32_idx(_virt_)], &(_ptb_)[pt32_idx(_virt_)] )


/*************************************************/
/**    Initializes PTE with identity mapping    **/
/**                                             **/
/**    PTE entries are aligned with 0x10        **/
/**                                             **/
/**    _ptb_ : address of PTB                   **/
/**    _virt_ : virtual address to map          **/
/*************************************************/
#define ptb_identity_pte(_ptb_,_virt_)				\
	ptb_pte( &(_ptb_)[pt32_idx(_virt_)], _virt_ )


/**************************************************/
/**    Initializes PTE for given mapping         **/
/**                                              **/
/**    Virtual -> Physical address mapping is    **/
/**    forced by user who must specifies both    **/
/**                                              **/
/**     PTE entries are aligned with 0x10        **/
/**                                              **/
/**    _ptb_ : address of PTB                    **/
/**    _virt_ : virtual address to map           **/
/**    _phys_ : physical address to map          **/
/**************************************************/
#define ptb_forced_pte(_ptb_,_virt_,_phys_)				\
	ptb_pte( &(_ptb_)[pt32_idx(_virt_)], _phys_ )

#endif