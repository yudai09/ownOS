#include "kcom.h"
#include "syscall.h"
#include "task.h"

extern "C" {void taskPman();}

void taskPman(){
  kprintf("taskPman() \n");
  Message message;
  message.m1.p1=333;
  //  sys_send(PID_SYSTASK,&message);
  kprintf("taskPman() for(;;);\n");
  //  sys_wait_interrupt(1);
  for(;;){

  }
}
