#include "graphic.h"

void init_palette(void)
{
	static unsigned char table_rgb[16 * 3] = {
		0x00, 0x00, 0x00,	/*  0:�� */
		0xff, 0x00, 0x00,	/*  1:���뤤�� */
		0x00, 0xff, 0x00,	/*  2:���뤤�� */
		0xff, 0xff, 0x00,	/*  3:���뤤���� */
		0x00, 0x00, 0xff,	/*  4:���뤤�� */
		0xff, 0x00, 0xff,	/*  5:���뤤�� */
		0x00, 0xff, 0xff,	/*  6:���뤤�忧 */
		0xff, 0xff, 0xff,	/*  7:�� */
		0xc6, 0xc6, 0xc6,	/*  8:���뤤���� */
		0x84, 0x00, 0x00,	/*  9:�Ť��� */
		0x00, 0x84, 0x00,	/* 10:�Ť��� */
		0x84, 0x84, 0x00,	/* 11:�Ť����� */
		0x00, 0x00, 0x84,	/* 12:�Ť��� */
		0x84, 0x00, 0x84,	/* 13:�Ť��� */
		0x00, 0x84, 0x84,	/* 14:�Ť��忧 */
		0x84, 0x84, 0x84	/* 15:�Ť����� */
	};
	set_palette(0, 15, table_rgb);
	return;

	/* static char ̿��ϡ��ǡ����ˤ����Ȥ��ʤ�����DB̿������ */
}

void set_palette(int start, int end, unsigned char *rgb)
{
	int i, eflags;
    //	eflags = io_load_eflags();	/* �����ߵ��ĥե饰���ͤ�Ͽ���� */
	cli_asm(); 					/* ���ĥե饰��0�ˤ��Ƴ����߶ػߤˤ��� */
	io_out8_asm(0x03c8, start);
	for (i = start; i <= end; i++) {
		io_out8(0x03c9, rgb[0] / 4);
		io_out8(0x03c9, rgb[1] / 4);
		io_out8(0x03c9, rgb[2] / 4);
		rgb += 3;
	}
    //	io_store_eflags(eflags);	/* �����ߵ��ĥե饰�򸵤��᤹ */
	return;
}

void boxfill8(unsigned char *vram, int xsize, unsigned char c, int x0, int y0, int x1, int y1)
{
	int x, y;
	for (y = y0; y <= y1; y++) {
		for (x = x0; x <= x1; x++)
			vram[y * xsize + x] = c;
	}
	return;
}

void init_screen8(char *vram, int x, int y)
{
	boxfill8(vram, x, COL8_008484,  0,     0,      x -  1, y - 29);
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 28, x -  1, y - 28);
	boxfill8(vram, x, COL8_FFFFFF,  0,     y - 27, x -  1, y - 27);
	boxfill8(vram, x, COL8_C6C6C6,  0,     y - 26, x -  1, y -  1);

	boxfill8(vram, x, COL8_FFFFFF,  3,     y - 24, 59,     y - 24);
	boxfill8(vram, x, COL8_FFFFFF,  2,     y - 24,  2,     y -  4);
	boxfill8(vram, x, COL8_848484,  3,     y -  4, 59,     y -  4);
	boxfill8(vram, x, COL8_848484, 59,     y - 23, 59,     y -  5);
	boxfill8(vram, x, COL8_000000,  2,     y -  3, 59,     y -  3);
	boxfill8(vram, x, COL8_000000, 60,     y - 24, 60,     y -  3);

	boxfill8(vram, x, COL8_848484, x - 47, y - 24, x -  4, y - 24);
	boxfill8(vram, x, COL8_848484, x - 47, y - 23, x - 47, y -  4);
	boxfill8(vram, x, COL8_FFFFFF, x - 47, y -  3, x -  4, y -  3);
	boxfill8(vram, x, COL8_FFFFFF, x -  3, y - 24, x -  3, y -  3);
	return;
}

void putfont8(char *vram, int xsize, int x, int y, char c, char *font)
{
	int i;
	char *p, d /* data */;
	for (i = 0; i < 16; i++) {
		p = vram + (y + i) * xsize + x;
		d = font[i];
		if ((d & 0x80) != 0) { p[0] = c; }
		if ((d & 0x40) != 0) { p[1] = c; }
		if ((d & 0x20) != 0) { p[2] = c; }
		if ((d & 0x10) != 0) { p[3] = c; }
		if ((d & 0x08) != 0) { p[4] = c; }
		if ((d & 0x04) != 0) { p[5] = c; }
		if ((d & 0x02) != 0) { p[6] = c; }
		if ((d & 0x01) != 0) { p[7] = c; }
	}
	return;
}

void putfonts8_asc(char *vram, int xsize, int x, int y, char c, unsigned char *s)
{
	extern char hankaku[4096];
	for (; *s != 0x00; s++) {
		putfont8(vram, xsize, x, y, c, hankaku + *s * 16);
		x += 8;
	}
	return;
}



void putblock8_8(char *vram, int vxsize, int pxsize,
	int pysize, int px0, int py0, char *buf, int bxsize)
{
	int x, y;
	for (y = 0; y < pysize; y++) {
		for (x = 0; x < pxsize; x++) {
			vram[(py0 + y) * vxsize + (px0 + x)] = buf[y * bxsize + x];
		}
	}
	return;
}

Sheet *wallpaper;
void initWallPaper(void){
  struct BOOTINFO *binfo =(struct BOOTINFO *)ADR_BOOTINFO;
  char *pics = (char *)allocMemory(sizeof(char)*binfo->scrnx*binfo->scrny);
  int i;
  wallpaper = allocSheet();
  if(wallpaper ==NULL){
    error("initWallPaper");
  }
  for(i=0;i<binfo->scrnx*binfo->scrny;i++){
    if((i%binfo->scrnx)%100==0||(i/binfo->scrnx)%100==0)
      pics[i]=COL8_FFFFFF;
    else if((i%binfo->scrnx)%10==0||(i/binfo->scrnx)%10==0)
      pics[i]=COL8_FF0000;
    else
      pics[i]=COL8_848484;
  }

  if(setSheet(wallpaper,0,0,binfo->scrnx,binfo->scrny,allVisible,pics)==-1){
    error("initWallPaper-2");
  }
  addSheetOnDisp(wallpaper);
  testReflesh();
}
