#ifndef PAGEMEM_H
#define PAGEMEM_H

#include <cr.h>
#include <pagemem.h>
#include <extend/memory.h>


#define cr3_pgd_set(_cr3_,_pgd_)		\
	{(_cr3_)->raw = (offset_t)_pgd_;}

#define cr0_pg_en(_cr0_)				\
	{(_cr0_)->raw |= CR0_PG;}

#define pgd_krn_pde(_pde_,_ptb_)				\
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

#define pgd_usr_pde(_pde_,_ptb_)				\
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

#define ptb_krn_pte(_pte_,_phys_)							\
	pg_set_entry(_pte_, PG_RW, pg_4K_nr(_phys_))

#define ptb_usr_pte(_pte_,_phys_)							\
	pg_set_entry(_pte_, PG_RW | PG_USR, pg_4K_nr(_phys_))


/*************************************************/
/**    Initializes PDE with identity mapping    **/
/**                                             **/
/**    PDE entries are aligned with 0x1000      **/
/**                                             **/
/**    _pgd_ : address of PGD                   **/
/**    _ptb_ : address of PTB                   **/
/**    _virt_ : virtual address to map          **/
/*************************************************/
#define pgd_krn_identity_pde(_pgd_,_ptb_,_virt_)							\
	pgd_krn_pde( &(_pgd_)[pd32_idx(_virt_)], &(_ptb_)[pt32_idx(_virt_)] )

#define pgd_usr_identity_pde(_pgd_,_ptb_,_virt_)							\
	pgd_usr_pde( &(_pgd_)[pd32_idx(_virt_)], &(_ptb_)[pt32_idx(_virt_)] )


/*************************************************/
/**    Initializes PTE with identity mapping    **/
/**                                             **/
/**    PTE entries are aligned with 0x10        **/
/**                                             **/
/**    _ptb_ : address of PTB                   **/
/**    _virt_ : virtual address to map          **/
/*************************************************/
#define ptb_krn_identity_pte(_ptb_,_virt_)				\
	ptb_krn_pte( &(_ptb_)[pt32_idx(_virt_)], _virt_ )

#define ptb_usr_identity_pte(_ptb_,_virt_)				\
	ptb_usr_pte( &(_ptb_)[pt32_idx(_virt_)], _virt_ )


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
#define ptb_krn_forced_pte(_ptb_,_virt_,_phys_)			\
	ptb_krn_pte( &(_ptb_)[pt32_idx(_virt_)], _phys_ )

#define ptb_usr_forced_pte(_ptb_,_virt_,_phys_)			\
	ptb_usr_pte( &(_ptb_)[pt32_idx(_virt_)], _phys_ )


/**************************************/
/**    Returns the address of the    **/
/**        nth PGD in memory         **/
/**************************************/
#define nth_pgd(n)	(pde32_t*)(PGD_START + n*PGD_SIZE)


/**************************************/
/**    Returns the address of the    **/
/**        nth PTB in memory         **/
/**************************************/
#define nth_ptb(n)	(pte32_t*)(PTB_START + n*PTB_SIZE)


/******************************************************/
/**    Maps memory with identity mapping             **/
/**                                                  **/
/**    _pgd_   : address of PGD to use               **/
/**    _ptb_   : address of PTB to use               **/
/**    _start_ : start address of desired mapping    **/
/**    _end_   : last byte of the end address of     **/
/**              desired mapping                     **/
/**                                                  **/
/**    Example : to map Kernel space :               **/
/**    krn_identity_map(KERNEL_START, KERNEL_END)    **/
/******************************************************/
#define krn_identity_map(_pgd_,_ptb_,_start_,_end_)										\
	{																		\
		for(offset_t p = _start_; p <= _end_; p += PGD_OFFSET) {			\
			pgd_krn_identity_pde((_pgd_), (_ptb_), p);						\
		}																	\
		for(offset_t p = _start_; p <= _end_; p += PTB_OFFSET) {			\
			ptb_krn_identity_pte((_ptb_), p);								\
		}																	\
	}

#define usr_identity_map(_pgd_,_ptb_,_start_,_end_)							\
	{																		\
		for(offset_t p = _start_; p <= _end_; p += PGD_OFFSET) {			\
			pgd_usr_identity_pde((_pgd_), (_ptb_), p);						\
		}																	\
		for(offset_t p = _start_; p <= _end_; p += PTB_OFFSET) {			\
			ptb_usr_identity_pte((_ptb_), p);								\
		}																	\
	}


/***********************************************************/
/**    Maps memory with forced mapping                    **/
/**                                                       **/
/**    _pgd_   : address of PGD to use                    **/
/**    _ptb_   : address of PTB to use                    **/
/**    _start_ : logical start address of mapping         **/
/**    _start_ : physical start address of mapping        **/
/**    _size_  : size in bytes of the desired mapping     **/
/***********************************************************/
#define krn_forced_map(_pgd_,_ptb_,_lstart_,_pstart_,_size_)						\
	{																				\
		for(offset_t offset = 0; offset <= _size_ - 1; offset += PGD_OFFSET) {		\
			pgd_krn_identity_pde((_pgd_), (_ptb_), _lstart_ + offset);				\
		}																			\
		for(offset_t offset = 0; offset <= _size_ - 1; offset += PTB_OFFSET) {		\
			ptb_krn_forced_pte((_ptb_), _lstart_ + offset, _pstart_ + offset);		\
		}																			\
	}


void init_pgd(void);

#endif
