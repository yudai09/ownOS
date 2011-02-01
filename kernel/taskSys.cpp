#include "kcom.h"
//#include"asmfunc.h"
//#include "Ipc.h"
#include "task.h"
#include "syscall.h"
#include "taskSys.h"
#include "KGlobal.h"

extern "C" {void taskSys();}

Message m_wait_interrupt;


void init_taskSys(){
  m_wait_interrupt.m1.p1=WAIT_INTERRUPT;
}
void taskSys(void){

  kprintf("taskSys\n");
  init_taskSys();

  Message message;

  message.m1.p1=111;
  //  sys_recv(PID_PMAN,&message);

  kprintf("taskSys() for(;;); \n");
  for(;;){
    //    sys_recv();

    //sys_recv(PID_IDLE,&message);
  }
}
