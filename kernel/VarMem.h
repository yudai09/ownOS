/*
 * var_memory.h
 *
 *  Created on: 2009/11/19
 *      Author: yudai
 */

#ifndef VAR_MEMORY_H_
#define VAR_MEMORY_H_

#include"kcom.h"
#include"PFAllocator.h"



class VarMem {
  //ページテーブル一つが持っているエントリ数(1024)
  const static u32_t nrEntry=0x400;

  const static u32_t dir_entrySize = 0x400000;//4MB
  const static u32_t page_entrySize = 0x1000;//4KB

  inline bool isEntryExist(const entry_t entry){
    return ((u32_t)entry&0x01)==0x01;
  }
  inline entry_t *caddrEntry(const entry_t entry){
    return (u32_t *)(entry & ~(0xfff));
  }
 public:
  const static u16_t invalidpage=0x000;
  const static u16_t kernelpage=0x011;
  const static u16_t systempage=0x013;
  const static u16_t userpage  =0x017;

  entry_t *kPdir;
  entry_t **kPtables;
  entry_t *v2ptable;

  inline entry_t *mapP2V_4k(u32_t *var,u8_t nb){
    u32_t page=ALIGN_4K((u32_t)var);
    u32_t offset=MOD_4K((u32_t)var);
    editEntry((u32_t *)v2ptable+nb,(u32_t)page,systempage);
    return (entry_t *)(0xffc00000+0x1000*nb+offset);
  }
  void initPaging(void);
  void copyKernelDir(entry_t *pdir);
  void handle_pageFault(u32_t cr2,u32_t errorcode,u32_t eip);
  void editEntry(entry_t *entry,u32_t pointer,u16_t type);
  void enableSpace(u32_t *virAddr,u32_t size,entry_t *pdir,const u16_t type);
  void disableAllEntry(entry_t *table);
  u32_t *vir2phy(u32_t *virAddr,entry_t *pdir=NULL);
  VarMem();
};

#endif /* VAR_MEMORY_H_ */
