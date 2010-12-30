/*
 * var_memory.h
 *
 *  Created on: 2009/11/19
 *      Author: yudai
 */

#ifndef VAR_MEMORY_H_
#define VAR_MEMORY_H_

#include"PageFrameAllocator.h"

//#define kernelPage 0x001//(for non cache)
//#define userPage   0x007

class Var_memory {

	PFAllocator pagedir_allocater;
	const static int PAGE_SIZE=0x1000;//ページテーブルは１エントリで４KBまでを管理
	const static int DIR_CAPACITY=0x400000;//ページディレクトリは１エントリで４MBまでを管理
	const static int NumEntry=0x400;



	void init_paging(void);
public:
	u32_t *kernelpagedir;
	u32_t *kernelpagetable0;
	const static u16_t invalidpage=0x000;
	const static u16_t kernelpage=0x011;
	const static u16_t systempage=0x013;
	const static u16_t userpage  =0x017;
	void MakeEntry(u32_t *entry,u32_t pointer,int type);
	void disableAllEntry(unsigned int *table);
	Var_memory();
	u32_t *makePageDir();
	u32_t freePageDir(u32_t *addr);
	//u32_t var2physic(u32_t vaddr,u32_t *pagedir);
	void shmWIthK(const u32_t kmem_b,const u32_t umem_b,size_t size,u16_t type,u32_t *pageDir);
	void setPagePriv_inKernel(const u32_t mem_begin,const u32_t mem_end,u16_t priv,u32_t *pageDir);
	void setPagePriv_inUser(const u32_t mem_begin,const u32_t mem_end,u16_t priv,u32_t *pageDir);
	void write_phymem(u32_t phyaddr,const void *buff,size_t size);
	void read_phymem(u32_t phyaddr,void *buff,size_t size);
	u32_t isValidEntry(u32_t entry);
	u32_t Var2Phys(u32_t viraddr);
};

#endif /* VAR_MEMORY_H_ */
