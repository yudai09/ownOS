#include"kcom.h"
#include"tk_printf.h"
#include"asmfunc.h"
#include"KGlobal.h"
extern "C" {void kinit( void* mbd, unsigned int magic );}
void mbi_read(void *addr,unsigned int magic);

void init_mouse_keyboard(void);
void init_pit(void);
void init_tasks(void);
void init_kmalloc(void);
extern "C" {void init_pic(void);}//NASM

void kinit( void* mbd, unsigned int magic )
{

  kprintf=tk_printf;//一時的な出力関数を設定
  mbi_read(mbd,magic);//実行時情報を表示＋空き領域リストを作成

  /*空き領域リストの表示*/
  //PFAllocator::Seg *p=pFAllocator.seg_free;

  tss.initTSS();//順番を変更してはならない
  idt.initIDT();
  gdt.initGDT();
  //initTSSに入れたいが初期化の順序が問題
  idt.lidt();
  gdt.lgdt();
  ltr_asm(Desc::TssDesc);

  init_pit();
  init_pic();
  init_mouse_keyboard();
  varMem.initPaging();
  init_kmalloc();
  //  io_out8_asm(0x21,0xFC);//

  io_out8_asm(0x21,0xFE);//
  int *x=(int *)kmalloc(sizeof(int));
  *x=1;
  kprintf("x= %d &x %x \n",*x,x);

  init_tasks();
  kprintf("initEnd__!!\n");
  for(;;);
}
