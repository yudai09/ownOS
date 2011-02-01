/*
 * KGlobal.cpp
 *
 *  Created on: 2010/03/07
 *      Author: yudai
 *
 *   ”KGlobal.h”で宣言
 */

#include"KGlobal.h"

#include"irq.h"
#include"isr.h"

//#include"Irq"

#define IGNORE ignore
BootInfo bootinfo;
u32_t tssaddr;
u32_t idtaddr;
u32_t gdtaddr;
//カーネルオブジェクト
PFAllocator pFAllocator;
Idt idt;
Gdt gdt;
Tss tss;
VarMem varMem;
Timer timer;
PManager pManager;
u32_t *next_sf;
FileSystem *fs;
//出力方法がブート時とそれ以外でことなるので各時点で下のポインターを変更する
void (*kprintf )(const char *format, ...);
void (*isr[0x100])()={
  isr00,isr01,isr02,isr03,isr04,isr05,isr06,isr07,
  isr08,isr09,isr0a,isr0b,isr0c,isr0d,isr0e,isr0f,
  isr10,isr11,isr12,isr13,isr14,isr15,isr16,isr17,
  isr18,isr19,isr1a,isr1b,isr1c,isr1d,isr1e,isr1f,
  isr20,isr21,isr22,isr23,isr24,isr25,isr26,isr27,
  isr28,isr29,isr2a,isr2b,isr2c,isr2d,isr2e,isr2f,
  isr30,isr31,isr32,isr33,isr34,isr35,isr36,isr37,
  isr38,isr39,isr3a,isr3b,isr3c,isr3d,isr3e,isr3f,
  isr40,isr41,isr42,isr43,isr44,isr45,isr46,isr47,
  isr48,isr49,isr4a,isr4b,isr4c,isr4d,isr4e,isr4f,
  isr50,isr51,isr52,isr53,isr54,isr55,isr56,isr57,
  isr58,isr59,isr5a,isr5b,isr5c,isr5d,isr5e,isr5f,
  isr60,isr61,isr62,isr63,isr64,isr65,isr66,isr67,
  isr68,isr69,isr6a,isr6b,isr6c,isr6d,isr6e,isr6f,
  isr70,isr71,isr72,isr73,isr74,isr75,isr76,isr77,
  isr78,isr79,isr7a,isr7b,isr7c,isr7d,isr7e,isr7f,
  isr80,isr81,isr82,isr83,isr84,isr85,isr86,isr87,
  isr88,isr89,isr8a,isr8b,isr8c,isr8d,isr8e,isr8f,
  isr90,isr91,isr92,isr93,isr94,isr95,isr96,isr97,
  isr98,isr99,isr9a,isr9b,isr9c,isr9d,isr9e,isr9f
};
/*void test(void *){
  kprintf("test \n");
  }*/
/*動的に呼び出す関数を変更するための関数ポインター群*/
void (*intr_func[0x100])(void *)={
		divide_exception,		debuggingInterrupt,	IGNORE,	IGNORE,
		IGNORE,	outofbundsException,	invalidInstruction,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	noSegmentExeption,
		stackException,	generalException,		pageFault,		IGNORE,
		//0x10;
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		//0x20;
		timerHandler,	Irq_notify<1>,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	Irq_notify<14>,Irq_notify<15>,
		//0x30;
		kernelCall,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		//0x40;
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		//0x50
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		//0x60
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		//0x70
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		//0x80
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		//0x90
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
		IGNORE,	IGNORE,	IGNORE,	IGNORE,
};
