#include"kcom.h"
#include"KGlobal.h"
#include"asmfunc.h"
Vram::VramInfo::VramInfo(){

      unsigned char rgb[nr_color * 3] = {
        0x00, 0x00, 0x00,	/*  0:黒 */
        0xff, 0x00, 0x00,	/*  1:明るい赤 */
        0x00, 0xff, 0x00,	/*  2:明るい緑 */
        0xff, 0xff, 0x00,	/*  3:明るい黄色 */
        0x00, 0x00, 0xff,	/*  4:明るい青 */
        0xff, 0x00, 0xff,	/*  5:明るい紫 */
        0x00, 0xff, 0xff,	/*  6:明るい水色 */
        0xff, 0xff, 0xff,	/*  7:白 */
        0xc6, 0xc6, 0xc6,	/*  8:明るい灰色 */
        0x84, 0x00, 0x00,	/*  9:暗い赤 */
        0x00, 0x84, 0x00,	/* 10:暗い緑 */
        0x84, 0x84, 0x00,	/* 11:暗い黄色 */
        0x00, 0x00, 0x84,	/* 12:暗い青 */
        0x84, 0x00, 0x84,	/* 13:暗い紫 */
        0x00, 0x84, 0x84,	/* 14:暗い水色 */
        0x84, 0x84, 0x84	/* 15:暗い灰色 */
      };
      for(int i=0;i<nr_color*3;i++){
        table_rgb[i]=rgb[i];      
      }
}
void Vram::set_qemuvga0reg(int reg, int dat)
{
  io_out16_asm(0x01ce, reg);
  io_out16_asm(0x01cf, dat);
  return;
}


void Vram::init_qemuvga0(int x, int y, int c, int flag)
{
  kprintf("init vga \n");

  set_qemuvga0reg(0x0004, 0x0000);
  set_qemuvga0reg(0x0001, x);
  set_qemuvga0reg(0x0002, y);
  set_qemuvga0reg(0x0003, c); /* 4, 8, 15, 16, 24, 32 */
  set_qemuvga0reg(0x0005, 0x0000);
  set_qemuvga0reg(0x0004, flag); /* リニアアクセスモードでVRAMの初期化をするなら0x41 */
  /* bit7 : VRAM初期化抑制, bit6 : リニアアクセスモード, bit0 : 設定有効 */
  vram_info->vx = x;
  vram_info->vy = y;

  if ((flag & 0x40) == 0) {
    vram_info->vram = (unsigned char *) 0x000a0000;
  } else {
    vram_info->vram = (unsigned char *) 0xe0000000;
  }
  // binfo->scrnx = x;
  // binfo->scrny = y;
  // binfo->vmode = c;
  // if ((flag & 0x40) == 0) {
  //     binfo->vram = (char *) 0x000a0000;
  // } else {
  //     binfo->vram = (char *) 0xe0000000;
  // }

  return;
}


void Vram::set_palette(int start, int end)
{
  int i;//, eflags;
  kprintf("set_palette \n");
  volatile u32_t eflags = io_load_eflags();	/* 割り込み許可フラグの値を記録する */
  kprintf("flags %x \n",eflags);
  cli_asm(); 					/* 許可フラグを0にして割り込み禁止にする */

  unsigned char *rgb=vram_info->table_rgb;
  io_out8_asm(0x03c8, start);
  for (i = start; i <= end; i++) {
    io_out8_asm(0x03c9, rgb[0] / 4);
    io_out8_asm(0x03c9, rgb[1] / 4);
    io_out8_asm(0x03c9, rgb[2] / 4);
    rgb += 3;
  }

  sti_asm();
  //  io_store_eflags(eflags);	/* 割り込み許可フラグを元に戻す */
  return;
}


void Vram::init_palette(void)
{
  set_palette(0, vram_info->nr_color);
  return;
}

void Vram::boxfill8(unsigned char c, int x0, int y0, int x1, int y1)
{
	int x, y;
	for (y = y0; y <= y1; y++) {
		for (x = x0; x <= x1; x++)
			vram_info->vram[y * vram_info->vx + x] = c;
	}
	return;
}

void Vram::init_vga(){

  vram_info = new VramInfo();

  init_palette();
  init_qemuvga0(1024, 768, 8, 0xc1);

  boxfill8(0x00,0,0,1024,768);
  boxfill8(0x01,300,200,400,300);
  // for(int i=0;i<300*400;i++)
  //   vram_info->vram[i]=0x11;
  //  vram_info->vram[0]=1;
}
