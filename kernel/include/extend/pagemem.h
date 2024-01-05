#ifndef __EXTEND_PAGEMEM_H__
#define __EXTEND_PAGEMEM_H__

#include <cr.h>
#include <pagemem.h>
#include <extend/memory.h>
#include <extend/stacks.h>

// #define min(a,b)				((a) <= (b) ? (a) : (b))

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
#define nth_user_pgds(n)		((pde32_t*)(PGD_START + (n + 1)*PGD_SIZE))


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
#define set_krn_pde(_pde_,_ptb_)				\
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

#define set_usr_pde(_pde_,_ptb_)				\
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
/**    Initializes PTE with given address     **/
/**                                           **/
/**    PTE entries are aligned with 0x0010    **/
/**                                           **/
/**    _pte_   : address of PTE to set up     **/
/**    _phys_  : physical address to put      **/
/**              in PTE                       **/
/***********************************************/
#define set_krn_pte(_pte_,_phys_)							\
	pg_set_entry(_pte_, PG_RW, pg_4K_nr(_phys_))

#define set_usr_pte(_pte_,_phys_)							\
	pg_set_entry(_pte_, PG_RW | PG_USR, pg_4K_nr(_phys_))


/******************************************************/
/**    Maps memory with identity mapping             **/
/**                                                  **/
/**    _pgd_   : address of used PGD                 **/
/**    _start_ : start address of desired mapping    **/
/**    _end_   : last byte of the end address of     **/
/**              desired mapping                     **/
/**                                                  **/
/**    /!\ _end_ is the address of the first byte    **/
/**    AFTER the mapping                             **/
/**                                                  **/
/**    Prefix specifies pages' privilege level       **/
/******************************************************/
#define krn_identity_map(_pgd_,_start_,_end_)													\
	{																							\
		pde32_t *pde;																			\
		pte32_t *ptb, *pte;																		\
		for(offset_t p = (offset_t)(_start_); p < (offset_t)(_end_); p += PDE_OFFSET) {			\
			pde = &(_pgd_)[pd32_idx(p)];														\
			ptb = pde->p ? (pte32_t*)(pde->addr << 12) : pop_ptb();								\
			set_krn_pde( pde, ptb );															\
																								\
			for(offset_t t = p; t < min(p + PDE_OFFSET - 1, _end_); t += PTE_OFFSET) {			\
				pte = &ptb[pt32_idx(t)];														\
				set_krn_pte( pte, t );															\
			}																					\
		}																						\
	}
	
#define usr_identity_map(_pgd_,_start_,_end_)													\
	{																							\
		pde32_t *pde;																			\
		pte32_t *ptb, *pte;																		\
		for(offset_t p = (offset_t)(_start_); p < (offset_t)(_end_); p += PDE_OFFSET) {			\
			pde = &(_pgd_)[pd32_idx(p)];														\
			ptb = pde->p ? (pte32_t*)(pde->addr << 12) : pop_ptb();								\
			set_usr_pde( pde, ptb );															\
																								\
			for(offset_t t = p; t < min(p + PDE_OFFSET, _end_); t += PTE_OFFSET) {				\
				pte = &ptb[pt32_idx(t)];														\
				set_usr_pte( pte, t );															\
			}																					\
		}																						\
	}


/***********************************************************/
/**    Maps memory with forced mapping                    **/
/**                                                       **/
/**    _pgd_    : address of PGD to use                   **/
/**    _lstart_ : logical start address of mapping        **/
/**    _pstart_ : physical start address of mapping       **/
/**    _size_   : size in bytes of the desired mapping    **/
/***********************************************************/
#define krn_forced_map(_pgd_,_lstart_,_pstart_,_size_)																			\
	{																															\
		pde32_t *pde;																											\
		pte32_t *ptb, *pte;																										\
		for(offset_t offset = 0; offset < _size_; offset += PDE_OFFSET) {														\
			pde = &(_pgd_)[pd32_idx(( offset + (offset_t)(_lstart_) ))];														\
			ptb = pde->p ? (pte32_t*)(pde->addr << 12) : pop_ptb();																\
			set_krn_pde( pde, ptb );																							\
																																\
			for(offset_t t_offset = offset; t_offset < min(offset + PDE_OFFSET, _size_); t_offset += PTE_OFFSET) {				\
				pte = &ptb[pt32_idx(( offset + (offset_t)(_lstart_) ))];														\
				set_krn_pte( pte, (t_offset  + (offset_t)(_pstart_)) );															\
			}																													\
		}																														\
	}

#define usr_forced_map(_pgd_,_lstart_,_pstart_,_size_)																			\
	{																															\
		pde32_t *pde;																											\
		pte32_t *ptb, *pte;																										\
		for(offset_t offset = 0; offset < _size_; offset += PDE_OFFSET) {														\
			pde = &(_pgd_)[pd32_idx(( offset + (offset_t)(_lstart_) ))];														\
			ptb = pde->p ? (pte32_t*)(pde->addr << 12) : pop_ptb();																\
			set_usr_pde( pde, ptb );																							\
																																\
			for(offset_t t_offset = offset; t_offset < min(offset + PDE_OFFSET, _size_); t_offset += PTE_OFFSET) {				\
				pte = &ptb[pt32_idx(( offset + (offset_t)(_lstart_) ))];														\
				set_usr_pte( pte, (t_offset  + (offset_t)(_pstart_)) );															\
			}																													\
		}																														\
	}

#define task_page_forced_map(_task_,_vaddr_,_paddr_) task_forced_map(_task_,_vaddr_,_paddr_, PTE_OFFSET)

void init_pgd(void);

// Inits PGD for the given task index
void init_task_pagemem(tidx task);

// Fills task PTB with the virtual and physical addresses
void task_forced_map(tidx task, offset_t virtual_address, offset_t physical_address, offset_t size);

// Flushes paging entries in usr_ptb and mirrors actions in krn_ptb
void flush_ptb(pte32_t *usr_ptb, pte32_t *krn_ptb);

// Flushes paging entries in usr_pgd and mirrors actions in krn_pgd
void flush_pgd(pde32_t *usr_pgd, pde32_t *krn_pgd);

// Flushes task's PGD and PTBs, clears corresponding entries in kernel's PGD
void clear_task_pagemem(tidx task);

#endif
