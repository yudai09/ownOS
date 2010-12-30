/*initialize timer*/
/*
timer frequency is 1.1932Mhz
setting pit_tnt0 1192(0x2e9c) let interval 10mhz
*/
#include"asmfunc.h"
#define PIT_CTRL	0x0043
#define PIT_CNT0	0x0040
void init_pit(void)
{
  io_out8_asm(PIT_CTRL, 0x34);
  io_out8_asm(PIT_CNT0, 0x9c);
  io_out8_asm(PIT_CNT0, 0x2e); 
  return;
}
