/*
 * var_memory.cpp
 *
 *  Created on: 2009/11/19
 *      Author: yudai
 */

#include"com.h"
#include"KGlobal.h"
#include"VarMem.h"
#include"asmfunc.h"
//#include"PFAllocator.h"
//#include"irq.h"

VarMem::VarMem()
{
}
void VarMem::disableAllEntry(entry_t *table){
  int i;
  for(i=0;i<nrEntry;i++)
    table[i]&=0x00;
}
//disregard under 12bit of pointer
//refer 486 p308
//disable entry before editEntry!!
void VarMem::editEntry(entry_t *entry,const u32_t pointer,const u16_t type){
  *entry = pointer & 0xfffff000;
  *entry |= type    & 0x00000fff;
}
//ストレートマッピング領域を作成 全物理メモリ領域を先頭からマッピング（物理メモリ＝仮想メモリな領域）
//全部マッピングするのは持ったないが、スワッピングするのも面倒なので
void VarMem::initPaging(void){
  int i;
  kPdir=(entry_t *)pFAllocator.allocChunk(1);//4kb

  kPtables=(entry_t **)pFAllocator.allocChunk(1);//4kb
  //メモリの最大アドレスまでマッピング（切り捨てあり）
  u32_t nr_valid_pages = bootinfo.mem_size/dir_entrySize;
  entry_t *big_frame = pFAllocator.allocChunk(nr_valid_pages);
  kprintf("nr_valid_pages %x \n",nr_valid_pages);
  for(int i=0;i<nr_valid_pages;i++){
    kPtables[i]=(entry_t *)((u32_t)big_frame+PFAllocator::ChunkSize*i);
  }
  //  kPtables[0]=(entry_t *)pFAllocator.allocChunk(1);//4kb
  disableAllEntry(kPdir);

  for(int i=0;i<nr_valid_pages;i++){
    disableAllEntry(kPtables[i]);
    editEntry(&kPdir[i],(u32_t)kPtables[i],systempage);
    for(int j=0;j<nrEntry;j++){
      editEntry(kPtables[i]+j,(u32_t)(j*0x1000),systempage);
    }
  }

  //ディレクトリの最後にページングのテーブルを設定するための特別なテーブルを設定
  editEntry(kPdir+0x3ff,(u32_t)v2ptable,systempage);
  paging_on_asm((u32_t)kPdir);
}
void VarMem::copyKernelDir(entry_t *pdir){
  disableAllEntry(pdir);
  u32_t nr_valid_pages = bootinfo.mem_size/dir_entrySize;
  //ストレートマップ領域のみコピー
  for(int i=0;i<nr_valid_pages;i++){
    pdir[i]=kPdir[i];
  }
}
#define PROTECTION_VIOLATION 0x000000001
#define WRITE   0x00000002
#define BY_USER 0x00000004

void VarMem::handle_pageFault(u32_t cr2,u32_t errorcode,u32_t eip){
  bool system = !(errorcode&BY_USER);
  /* if(system){
    if(BEGIN_STRAIGHT_MEM<cr2 && cr2<END_STRAIGHT_MEM){
      kprintf("straight mem \n");
    }
    }*/
}

//viraddr must be aligned 
//viraddr Virtual address to be enable,from viraddr to viraddr+size
//pdir Page directory 
//type kernel,system,or application
//実は必要ないかもしれない（ストレートマップ領域あるから）でも後回し・・・
// void VarMem::enableSpace(u32_t *virAddr,u32_t size,entry_t *pdir,u16_t type)
// {
//   entry_t *entryD;
//   entry_t *table;
//   entry_t *entryT;
//   //pdir may be virtual address
//   flashCache_asm();//flash cache memory 

//   for(int i=0;i<size/0x1000;i++){
//     u32_t where=(u32_t)virAddr+i*0x1000;
//     entryD=mapP2V_4k(&pdir[where/0x400000],0);//mapping temporary
//     if(!isEntryExist(*entryD)){//entry exist?
//       table=(entry_t *)pFAllocator.allocChunk(1);//table
//       editEntry(entryD,(u32_t)table,type);
//       disableAllEntry(mapP2V_4k(table,1));
//     }else{
//       table=caddrEntry(*entryD);//obtain address from entry
//     }
//     entryT=mapP2V_4k(&table[(where%0x400000)/0x1000],2);
//     if(!isEntryExist(*entryT)){
//       u32_t *s4k=(u32_t *)pFAllocator.allocChunk(1);//4k
//       editEntry(entryT,(u32_t)s4k,type);
//     }
//   }

//   flashCache_asm();
// }
void VarMem::enableSpace(u32_t *virAddr,u32_t size,entry_t *pdir,u16_t type)
{
  entry_t *entryD;
  entry_t *table;
  entry_t *entryT;
  //pdir may be virtual address
  entry_t aligned_size = CEIL(size,0x1000);
  kprintf("enablespace: varaddr %x->%x \n",(u32_t)virAddr,(u32_t)virAddr+aligned_size);
  flashCache_asm();//flash cache memory 
  for(int i=0;i<aligned_size/0x1000;i++){
    u32_t where=(u32_t)virAddr+i*0x1000;
    //    entryD=mapP2V_4k(&pdir[where/0x400000],0);//mapping temporary
    entryD=&pdir[where/0x400000];//mapping temporary
    //    kprintf("entryD %x \n",entryD);
    if(!isEntryExist(*entryD)){//entry exist?
      table=(entry_t *)pFAllocator.allocChunk(1);//table
      editEntry(entryD,(u32_t)table,type);
      disableAllEntry(table);
    }else{
      table=caddrEntry(*entryD);//obtain address from entry
    }
    //entryT=mapP2V_4k(&table[(where%0x400000)/0x1000],2);
    entryT=&table[(where%0x400000)/0x1000];
    if(!isEntryExist(*entryT)){
      u32_t *s4k=(u32_t *)pFAllocator.allocChunk(1);//4k
      editEntry(entryT,(u32_t)s4k,type);
    }
  }

  flashCache_asm();
}

/*仮想アドレスから物理アドレスに変換する
変換できなかった場合はNULLを返す*/
u32_t *VarMem::vir2phy(u32_t *virAddr,entry_t *pdir){
  entry_t *entryD,*entryT;
  entry_t *table;

  if(pdir==NULL)
    pdir = (entry_t *)pManager.pCurrent->mm->cr3;
  kprintf("pdir %x viraddr %x \n",pdir,virAddr);
  u32_t where = (u32_t)virAddr/0x400000;
  //  entryD = &pdir[(where/0x400000)];
  entryD = &pdir[where];
  if(!isEntryExist(*entryD)){
    kprintf("dir entry isn't exist %x %x %x\n",entryD,*entryD,where);
    return NULL;
  }
  table=caddrEntry(*entryD);
  entryT=&table[((u32_t)virAddr%0x400000)/0x1000];
  if(!isEntryExist(*entryT)){
    kprintf("table entry isn't exist \n");
    return NULL;
  }
  u32_t retaddr =  (u32_t)caddrEntry(*entryT);
  retaddr += (u32_t)virAddr&0xfff;
  return (u32_t*)retaddr;
}





