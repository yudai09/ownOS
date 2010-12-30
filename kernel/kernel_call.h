#ifndef _KERNEL_CALL_H_
#define _KERNEL_CALL_H_

#include"asmfunc.h"
//for include kernel_call_asm(...);

enum kernelcall{
  NR_K_SEND,
  NR_K_RECV,
  NR_K_FORK,
};

#endif
