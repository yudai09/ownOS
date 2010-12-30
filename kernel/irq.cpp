/*
 * irq.c
 *
 *  Created on: 2010/05/09
 *      Author: yudai
 */
#include"kcom.h"
#include"KGlobal.h"
#include"irq.h"
#include"kernel_call.h"
#include"Ipc.h"

void invalid(void *args){
  kprintf("invalid handler");
  for(;;);
}
void ignore(void *args)
{
  kprintf("IGNORE");
  for(;;);
}
void divide_exception(void *args)
{
  kprintf("divideExecption\n");
  for(;;);
}
void debuggingInterrupt(void *args)
{
  kprintf("debugginInterrupt\n");
  for(;;);
}
void outofbundsException(void *args)
{
  kprintf("outofbunds\n");
  for(;;);
}
void invalidInstruction(void *args)
{
  kprintf("invalidInstruction\n");
  for(;;);
}
void noSegmentExeption(void *args)
{
  kprintf("noSegmentException\n");
  for(;;);
}
void stackException(void *args)
{
  kprintf("stackException\n");
  for(;;);
}
void generalException(void *args)
{
  kprintf("generalException\n");
  for(;;);
}

void handle_pageFault(u32_t cr2,u32_t errorcode,u32_t eip);

void pageFault(void *args){
  u32_t *uargs = (u32_t *)&args;
  u32_t cr2 = uargs[0];
  u32_t errorcode = uargs[1];
  u32_t eip = uargs[2];
  kprintf("pageFault cr2 %x errorcode %x eip %x \n",cr2,errorcode,eip);
  kprintf("pid 0x%x \n",pManager.pCurrent->id);
  varMem.handle_pageFault(cr2,errorcode,eip);
  for(;;);
}
int fork();
//void kernelCall(u32_t arg1,u32_t arg2,u32_t arg3){
void kernelCall(void *args){
  //  kprintf("kernelCall %p %p %p \n",arg1,arg2,arg3);
//  u32_t *uargs=(u32_t *)&args;    
  stack_frame_s *regs = (stack_frame_s *)&args;
  kprintf("&args %x regs %x %x\n",&args,regs);

  u32_t uargs[4];
  uargs[0] = regs->ax;
  uargs[1] = regs->bx;
  uargs[2] = regs->cx;
  uargs[3] = regs->dx;
  kprintf("kernelcall %x %x %x \n",uargs[0],uargs[1],uargs[2]);

  switch(uargs[0]){
  case NR_K_SEND :
    {
      Message &mess=*(Message *)uargs[2];
      mess.id=pManager.pCurrent->id;
      //      Message mess(pManager.pCurrent->id);//myid
      Ipc::send(uargs[1],mess);
      break;
    }
  case NR_K_RECV :
    {
      Message &mess=*(Message *)uargs[2];
      mess.id=pManager.pCurrent->id;
      //      Message mess(pManager.pCurrent->id);//myid
      Ipc::recv(uargs[1],mess);
      kprintf("message.p1 %d \n",mess.m1.p1);
      break;
    }
  case NR_K_FORK :
    {
      pid_t pid=fork();
      kprintf("fork end %d \n",pid);
      regs->ax = pid;
      break;
    }
  default:
    break;
  }
}
