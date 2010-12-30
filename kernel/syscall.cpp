#include "kcom.h"
#include "syscall.h"
#include "kernel_call.h"
#include "Ipc.h"
#include "task.h"

#include "taskSys.h"

int sys_fork(){
  return kernel_call_asm(NR_K_FORK);
}

int sys_send(pid_t to,Message *message){
  return kernel_call_asm(NR_K_SEND,(u32_t)to,(u32_t)message);
}
int sys_recv(pid_t from,Message *message){
  return kernel_call_asm(NR_K_RECV,(u32_t)from,(u32_t)message);
}

int sys_wait_interrupt(int irq){
  Message message;
  message = m_wait_interrupt;
  message.m1.p2=irq;
  kernel_call_asm(NR_K_SEND,(u32_t)PID_SYSTASK,(u32_t)&message);
//wait for notify
  return kernel_call_asm(NR_K_RECV,(u32_t)PID_SYSTASK,(u32_t)&message);
}

/*
int syscall(u32_t nr,u32_t arg1,u32_t arg2,int arg3){
  Message args;
  args.m1.p1=arg1;
  args.m1.p2=arg2;
  args.m1.p3=arg3;
  kernel_call_asm(NR_KCALL_SEND,(u32_t)&args,0);
}
*/

