#ifndef PAGEMEM_H
#define PAGEMEM_H

#include <cr.h>
#include <pagemem.h>
#include <extend/memory.h>
#include <extend/stacks.h>


/**************************************/
/**    Returns the address of the    **/
/**        nth PGD in memory         **/
/**************************************/
#define nth_pgd_gbl(n)			((pde32_t*)(PGD_START + n*PGD_SIZE))


/**************************************/
/**    Returns the address of the    **/
/**        nth PTB in memory         **/
/**************************************/
#define nth_ptb_gbl(n)			((pte32_t*)(PTB_START + n*PTB_SIZE))


/******************************************/
/**    Returns the address of the        **/
/**    nth tasks's user PGDs             **/
/******************************************/
#define nth_user_pgds(n)		((pde32_t*)(PGD_START + (n*NUM_PGD_PER_TASK + NUM_PGD_KERNEL)*PGD_SIZE))


/******************************************/
/**    Returns the address of the        **/
/**    nth tasks's user PTBs             **/
/******************************************/
#define nth_user_ptbs(n)		((pte32_t*)(PTB_START + (n*NUM_PTB_PER_TASK + NUM_PTB_KERNEL)*PTB_SIZE))


/******************************************/
/**    Returns the address of the        **/
/**    nth PTB in a flat array of PTBs   **/
/******************************************/
#define nth_ptb(_ptbs_,n)		((pte32_t*)((offset_t)(_ptbs_) + n*PTB_SIZE))


/*******************************************/
/**    Set PGD addres in CR3 structure    **/
/*******************************************/
#define cr3_pgd_set(_cr3_,_pgd_)		{ (_cr3_)->raw = (offset_t)(_pgd_); }


/*********************************************/
/**    Enable paging in given CR0 struct    **/
/*********************************************/
#define cr0_pg_en(_cr0_)				{ (_cr0_)->raw |= CR0_PG; }


/***********************************************/
/**    Initializes PDE with given PTB         **/
/**                                           **/
/**    PDE entries are aligned with 0x1000    **/
/**                                           **/
/**    Suffix specifies pages' privilege      **/
/**    level                                  **/
/**                                           **/
/**    _pgd_  : address of PGD                **/
/**    _ptb_  : address of PTB                **/
/***********************************************/
#define set_pde_krn(_pde_,_ptb_)				\
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

#define set_pde_usr(_pde_,_ptb_)				\
	{											\
		(_pde_)->raw 	= (offset_t)_ptb_;		\
		(_pde_)->p		= 1;					\
		(_pde_)->rw		= 1;					\
		(_pde_)->lvl	= 1;					\
		(_pde_)->pwt	= 0;					\
		(_pde_)->pcd	= 0;					\
		(_pde_)->acc	= 0;					\
		(_pde_)->mbz	= 0;					\
		(_pde_)->avl	= 0;					\
	}

/***********************************************/
/**    Initializes PDE with given address     **/
/**                                           **/
/**    PTE entries are aligned with 0x0010    **/
/**                                           **/
/**    Suffix specifies pages' privilege      **/
/**    level                                  **/
/**                                           **/
/**    _pgd_  : address of PGD                **/
/**    _ptb_  : address of PTB                **/
/***********************************************/
#define ptb_krn_pte(_pte_,_phys_)							\
	pg_set_entry(_pte_, PG_RW, pg_4K_nr(_phys_))

#define ptb_usr_pte(_pte_,_phys_)							\
	pg_set_entry(_pte_, PG_RW | PG_USR, pg_4K_nr(_phys_))


/*************************************************/
/**    Initializes PDE                          **/
/**                                             **/
/**    PDE entries are aligned with 0x1000      **/
/**                                             **/
/**    _pgd_  : address of PGD                  **/
/**    _ptb_  : address of PTB                  **/
/**    _virt_ : virtual address to map          **/
/*************************************************/
#define pgd_set_krn_pde(_pgd_,_ptb_,_virt_)				\
	set_pde_krn( &(_pgd_)[pd32_idx(_virt_)], _ptb_ )

#define pgd_set_usr_pde(_pgd_,_ptb_,_virt_)				\
	set_pde_usr( &(_pgd_)[pd32_idx(_virt_)], _ptb_ )


/*************************************************/
/**    Initializes PTE with identity mapping    **/
/**                                             **/
/**    PTE entries are aligned with 0x10        **/
/**                                             **/
/**    _ptb_  : address of PTB                  **/
/**    _virt_ : virtual address to map          **/
/*************************************************/
#define ptb_set_krn_identity_pte(_ptb_,_virt_)											\
	ptb_krn_pte( _ptb_[pt32_idx(_virt_)], _virt_ )

#define ptb_set_usr_identity_pte(_ptbs_,_virt_)											\
	ptb_usr_pte( _ptb_[pt32_idx(_virt_)], _virt_ )


/**************************************************/
/**    Initializes PTE for given mapping         **/
/**                                              **/
/**    Virtual -> Physical address mapping is    **/
/**    forced by user who must specifies both    **/
/**                                              **/
/**     PTE entries are aligned with 0x10        **/
/**                                              **/
/**    _ptbs_ : address of PTBs                  **/
/**    _virt_ : virtual address to map           **/
/**    _phys_ : physical address to map          **/
/**************************************************/
#define ptb_set_krn_forced_pte(_ptbs_,_virt_,_phys_)								\
	ptb_krn_pte( &nth_ptb(_ptbs_, pd32_idx(_virt_))[pt32_idx(_virt_)], _phys_ )

#define ptb_set_usr_forced_pte(_ptbs_,_virt_,_phys_)								\
	ptb_usr_pte( &nth_ptb(_ptbs_, pd32_idx(_virt_))[pt32_idx(_virt_)], _phys_ )


/******************************************************/
/**    Maps memory with identity mapping             **/
/**                                                  **/
/**    _pgd_   : address of used PGD                 **/
/**    _ptbs_  : address of PGD's PTBs               **/
/**    _start_ : start address of desired mapping    **/
/**    _end_   : last byte of the end address of     **/
/**              desired mapping                     **/
/**                                                  **/
/**    Prefix specifies pages' privilege level       **/
/**                                                  **/
/**    Example: to map Kernel space:                 **/
/**    krn_identity_map(KERNEL_START, KERNEL_END)    **/
/******************************************************/
#define krn_identity_map(_pgd_,_start_,_end_)													\
	{																							\
		pde32_t* ptb = pop_ptb();																\
		for(offset_t p = (offset_t)(_start_); p <= (offset_t)(_end_); p += PDE_OFFSET) {		\
			pgd_set_krn_pde( ((pde32_t*)(_pgd_)), ptb, p );										\
		}																						\
		for(offset_t p = (offset_t)(_start_); p <= (offset_t)(_end_); p += PTE_OFFSET) {		\
			ptb_set_krn_identity_pte( ptb, p );													\
		}																						\
	}
#define krn_identity_map(_pgd_,_start_,_end_)													\
	{																							\
		pde32_t* ptb = pop_ptb();																\
		for(offset_t p = (offset_t)(_start_); p <= (offset_t)(_end_); p += PDE_OFFSET) {		\
			pgd_set_usr_pde( ((pde32_t*)(_pgd_)), ptb, p );										\
		}																						\
		for(offset_t p = (offset_t)(_start_); p <= (offset_t)(_end_); p += PTE_OFFSET) {		\
			ptb_set_usr_identity_pte( ptb, p );													\
		}																						\
	}


/***********************************************************/
/**    Maps memory with forced mapping                    **/
/**                                                       **/
/**    _pgd_    : address of PGD to use                   **/
/**    _ptbs_   : address of PGD's PTBs                   **/
/**    _lstart_ : logical start address of mapping        **/
/**    _pstart_ : physical start address of mapping       **/
/**    _size_   : size in bytes of the desired mapping    **/
/***********************************************************/
#define krn_forced_map(_pgd_,_ptbs_,_lstart_,_pstart_,_size_)																	\
	{																															\
		for(offset_t offset = 0; offset <= _size_ - 1; offset += PDE_OFFSET) {													\
			pgd_set_krn_pde( ((pde32_t*)(_pgd_)), ((pde32_t*)(_ptbs_)), ((offset_t)_lstart_ + offset) );						\
		}																														\
		for(offset_t offset = 0; offset <= _size_ - 1; offset += PTE_OFFSET) {													\
			ptb_set_krn_forced_pte( ((pde32_t*)(_ptbs_)), ((offset_t)_lstart_ + offset), ((offset_t)_pstart_ + offset) );		\
		}																														\
	}


void init_pgd(void);

#endif
