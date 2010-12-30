/*
 * Global.h
 *
 *  Created on: 2010/03/06
 *      Author: yudai
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_
#include"PFAllocator.h"
#include"Descriptor.h"
#include"Tss.h"
#include"VarMem.h"
#include"Timer.h"
#include"Process.h"
#include"com.h"

enum ZONE{
  BEGIN_STRAIGHT_MEM=0,
  END_STRAIGHT_MEM=0x2000000,//512MB
};
struct BootInfo{
  u32_t mem_size;
};
extern BootInfo bootinfo;
//各CPU設定用領域アドレス
extern u32_t tssaddr;
extern u32_t idtaddr;
extern u32_t gdtaddr;
//カーネルオブジェクト
extern PFAllocator pFAllocator;
extern Idt idt;
extern Gdt gdt;
extern Tss tss;
extern VarMem varMem;
extern Timer timer;
extern PManager pManager;
extern u32_t *next_sf;
extern void (*isr[0x100])();
extern void (*intr_func[0x100])(void *);
extern void (*kprintf )(const char *format, ...);

#endif /* GLOBAL_H_ */
