/*
 * descriptor.h
 *
 *  Created on: 2009/11/19
 *      Author: yudai
 */
#ifndef DESCRIPTOR_H_
#define DESCRIPTOR_H_
#include"com.h"
class Desc{//IDT,GDT双方が継承するクラス
public:
	static const u16_t KCSeg   = 0x08;
	static const u16_t KDSeg   = 0x10;
	static const u16_t TssDesc = 0x20;
	static const u16_t UCSeg   = 0x28;
	static const u16_t UDSeg   = 0x30;

	static const u16_t TCSeg = 0x40;
	static const u16_t TDSeg = 0x48;

	const static u32_t AR_CODE32 = 0x409a;
	const static u32_t AR_DATA32 = 0x4092;

	const static u32_t AR_TASKCODE32 = 0x40ba;
	const static u32_t AR_TASKDATA32 = 0x40b2;

	const static u32_t AR_USERCODE32 = 0x40fa;
	const static u32_t AR_USERDATA32 = 0x40f2;
	const static u32_t  TYPE_TSS     = 0x89; //TSSディスクリプタのタイプ;

	const static u32_t  AR_INT	   = 0x008e; //特権レベル０からの割り込みゲート;
	const static u32_t  AR_INT_p1  = 0x00ae; //特権レベル１からの割り込みゲート;
	const static u32_t  AR_INT_p3  = 0x00ee; //特権レベル１からの割り込みゲート;
	const static u32_t AR_TRAP	   = 0x008f;
	const static u32_t  AR_TRAP_p1 = 0x00af; //特権レベル１からのトラップゲート;

	void set_segdesc(struct seg_desc_s *desc,u32_t addr,u32_t  limit,u16_t  ar);
	void set_gatedesc(struct seg_desc_s *desc,u32_t addr,u32_t  limit,u16_t  ar);
	void set_gatedesc(struct seg_desc_s *desc,void *addr,u32_t  limit,u16_t  ar);
	void set_gatedesc(struct gate_desc_s *gd, u32_t offset, u32_t selector, u16_t ar);
};

class Gdt : public Desc{
  struct seg_desc_s *desc;//$B%G%#%9%/%j%W%?$N@hF,HVCO(B
public:
  Gdt();
  void lgdt();
  void initGDT();
};

class Idt : public Desc{
  struct gate_desc_s *desc;
 public:
  Idt();
  void lidt();
  void initIDT();
};

class Descriptor{
  Gdt gdt;
  Idt idt;
public:
};

#endif /* DESCRIPTOR_H_ */
