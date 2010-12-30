#include"KGlobal.h"
#include"Descriptor.h"
#include"Tss.h"
#include"isr.h"
#include"irq.h"
#include"asmfunc.h"
#define ISR(x) isr##x()
struct IDTR{
  u16_t s[3];//2byte*4
}idtr;
struct GDTR{
  u16_t s[3];//2byte*4
}gdtr;

Idt::Idt()
{
}
void Idt::lidt(){
  idtr.s[0]=0x100*8-1;
  u32_t *u32=(u32_t *)&(idtr.s[1]);
  *u32=idtaddr;
  /*
    idtr:
    dw	256*8-1
    dd	IDTADDR
  */
  //設定
  lidt_asm((unsigned int *)&idtr);
}
void Gdt::lgdt(){
  gdtr.s[0]=TDSeg+8;
  //gdtr.s[0]=256*8-1;
  u32_t *u32=(u32_t *)&(gdtr.s[1]);
  *u32=gdtaddr;
  /*
    idtr:
    dw	256*8-1
    dd	IDTADDR
  */
  //設定
  lgdt_asm((unsigned int *)&gdtr);
}
void Idt::initIDT()
{
  idtaddr=(u32_t)pFAllocator.allocChunk(10);
  desc=(struct gate_desc_s *)idtaddr;  
  struct gate_desc_s *p=desc;
  set_gatedesc(p,(unsigned int)invalid,KCSeg,AR_INT);
  struct gate_desc_s *invalid_g=p;//

  for(int i=1;i<0x100;i++){//IDTを全て初期化（全部ignoreへ飛ばすゲートにする）
    *p=*invalid_g;
    p++;
  }
  //ここから個別の割り込みを設定（ページフォルト、例外、ハード割り込み）
  for(int i=0;i<0x10;i++){
    set_gatedesc(desc+i,(u32_t)isr[i],KCSeg,AR_INT);
  } 
  set_gatedesc(desc+0x20,(u32_t)isr[0x20],KCSeg,AR_INT);
  set_gatedesc(desc+0x21,(u32_t)isr[0x21],KCSeg,AR_INT);
  set_gatedesc(desc+0x30,(u32_t)isr[0x30],KCSeg,AR_INT_p3);
  /*for(int i=0x20;i<0x30;i++){
    set_gatedesc(desc+i,(u32_t)isr[i],KCSeg,AR_INT);
    }*/
  
  
  /*
    set_gatedesc(desc+0x00,(unsigned int)inthandler00,KernelCodeSegment,AR_INT);
    set_gatedesc(desc+0x01,(unsigned int)inthandler01,KernelCodeSegment,AR_INT);
    set_gatedesc(desc+0x05,(unsigned int)inthandler05,KernelCodeSegment,AR_INT);
    set_gatedesc(desc+0x06,(unsigned int)inthandler06,KernelCodeSegment,AR_INT);
    set_gatedesc(desc+0x0B,(unsigned int)inthandler0b,KernelCodeSegment,AR_INT);
    set_gatedesc(desc+0x0C,(unsigned int)inthandler0c,KernelCodeSegment,AR_INT);
    set_gatedesc(desc+0x0d,(unsigned int)inthandler0d,KernelCodeSegment,AR_INT);
    set_gatedesc(desc+0x0e,(unsigned int)inthandler0e,KernelCodeSegment,AR_INT);

    set_gatedesc(desc+0x20,(unsigned int)inthandler20,KernelCodeSegment,AR_INT_p1);
    set_gatedesc(desc+0x21,(unsigned int)inthandler21,KernelCodeSegment,AR_INT);
    set_gatedesc(desc+0x26,(unsigned int)inthandler26,KernelCodeSegment,AR_INT);
    set_gatedesc(desc+0x30,(unsigned int)inthandler30,KernelCodeSegment,AR_INT_p3);//コピーカウント１
    set_gatedesc(desc+0x31,(unsigned int)inthandler31,KernelCodeSegment,AR_INT_p1);//コピーカウント１
  */
  //テスト！！！　プロセスができたら元にもどせ

  //set_gatedesc(desc+0x20,(unsigned int)timerHandler,KernelCodeSegment,AR_INT);
  //idtrの初期化
  //idtr.s[0]=256*8-1;
  
}
Gdt::Gdt()
{

}
void Gdt::initGDT(){
 gdtaddr=(u32_t)pFAllocator.allocChunk(10);
 for(int i=0;i<0x1000;i++)
   *((u32_t *)gdtaddr+i)=0;
 desc=(struct seg_desc_s *)gdtaddr;
 set_segdesc(&desc[KCSeg/0x8],(unsigned int)0x0000,(unsigned int)0xffffffff,AR_CODE32);
 set_segdesc(&desc[KDSeg/0x8],(unsigned int)0x0000,(unsigned int)0xffffffff,AR_DATA32);
 set_segdesc(&desc[UCSeg/0x8],(unsigned int)0x0000,(unsigned int)0xffffffff,AR_USERCODE32);
 set_segdesc(&desc[UDSeg/0x8],(unsigned int)0x0000,(unsigned int)0xffffffff,AR_USERDATA32);
 set_segdesc(&desc[TCSeg/0x8],(unsigned int)0x0000,(unsigned int)0xffffffff,AR_TASKCODE32);
 set_segdesc(&desc[TDSeg/0x8],(unsigned int)0x0000,(unsigned int)0xffffffff,AR_TASKDATA32);
 //tssディスクリプタの登録
 set_segdesc(&desc[TssDesc/0x08],(unsigned int)tssaddr,(unsigned int)sizeof(struct TSS32),(u16_t)TYPE_TSS);
}

void Desc::set_segdesc(struct seg_desc_s *desc,u32_t addr,u32_t  limit,u16_t  ar)
{

  if(limit > 0xfffff){
    ar |= 0x8000;
    limit /= 0x1000;
  }

  desc->limit_low   =  (u16_t)(limit & 0xffff);//address_low
  desc->limit_high  =  (u8_t)((limit >> 16)&0x0f)| ((ar >> 8) & 0xf0);
  desc->access_right = ar & 0xff;
  desc->base_low    =  (u16_t)(addr & 0xffff);
  desc->base_mid   =  (u8_t)((addr >> 16)&0xff);
  desc->base_high  =  (u8_t)((addr >> 24)&0xff);

  return;
}

void Desc::set_gatedesc(struct gate_desc_s *gd, u32_t offset, u32_t selector, u16_t ar)
{
  gd->offset_low   = offset & 0xffff;
  gd->selector     = selector;
  gd->count        = (ar >> 8) & 0xff;
  gd->access_right = ar & 0xff;
  gd->offset_high  = (offset >> 16) & 0xffff;
  return;
}

