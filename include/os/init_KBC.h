#include"asmfunc.h"

#define PORT_KEYDAT		0x0060
#define PORT_KEYCMD		0x0064

#define PORT_KEYSTA             0x0064
#define KEYSTA_SEND_NOTREADY	0x02
#define KEYCMD_WRITE_MODE	0x60
#define KBC_MODE	        0x47

static void wait_KBC_sendready(void)
{
	/* キーボードコントローラがデータ送信可能になるのを待つ */
  for (;;) {
    if ((io_in8_asm(PORT_KEYSTA) & KEYSTA_SEND_NOTREADY) == 0) {
      break;
    }
  }


  return;
}

void init_keyboard(void){
  wait_KBC_sendready();
  io_out8_asm(PORT_KEYCMD,KEYCMD_WRITE_MODE);
  wait_KBC_sendready();
  io_out8_asm(PORT_KEYDAT,KBC_MODE);
  return;
}

#define KEYCMD_SENDTO_MOUSE		0xd4
#define MOUSECMD_ENABLE			0xf4

void init_mouse(void){
   /* マウス有効 */
  wait_KBC_sendready();
  io_out8_asm(PORT_KEYCMD, KEYCMD_SENDTO_MOUSE);
  wait_KBC_sendready();
  io_out8_asm(PORT_KEYDAT, MOUSECMD_ENABLE);
  /* うまくいくとACK(0xfa)が送信されてくる */
 
  return;
}

void init_mouse_keyboard(void){
  init_keyboard();
  init_mouse();
}

