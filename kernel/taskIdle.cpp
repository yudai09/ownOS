#include"kcom.h"
#include "KGlobal.h"
//Never sleep
extern "C" {void taskIdle();}
void taskIdle(void){
  kprintf("taskIdle\n");
  kprintf("taskIdle() for(;;);\n");
  for(;;){

  }
}
