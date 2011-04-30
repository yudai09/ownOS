#include "kcom.h"
#include "syscall.h"
#include "kernel_call.h"
#include "Ipc.h"
#include "task.h"
#include "asmfunc.h"
#include "taskSys.h"

int sys_exec(char *filepath,enum priv p){
  return kernel_call_asm(NR_K_EXEC,(u32_t)filepath,p);
}
int sys_fork(){
  return kernel_call_asm(NR_K_FORK);
}

int sys_send(pid_t to,Message *message){
  return kernel_call_asm(NR_K_SEND,(u32_t)to,(u32_t)message);
}
int sys_recv(pid_t from,Message *message){
  return kernel_call_asm(NR_K_RECV,(u32_t)from,(u32_t)message);
}


