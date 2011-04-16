#include <syscall.h>
#include <kernel_call.h>
#include <Ipc.h>

int kernel_call_asm(u32_t nr,u32_t arg1=0,u32_t arg2=0,u32_t arg3=0){
  asm volatile(
	       "push %eax \n\t"
	       "push %ebx \n\t"
	       "push %ecx \n\t"
	       "push %edx \n\t"
	       "mov 20(%ebp),%edx\n\t "
	       "mov 16(%ebp),%ecx\n\t "
	       "mov 12(%ebp),%ebx\n\t "
	       "mov 8(%ebp),%eax\n\t "
	       "int $0x30\n\t "
	       "pop %edx\n\t "
	       "pop %ecx\n\t "
	       "pop %ebx\n\t "
	       "addl $4,%esp \n\t"
	       //"pop %eax\n\t "
	       );

}

int sys_send(pid_t to,Message *message){
  return kernel_call_asm(NR_K_SEND,(u32_t)to,(u32_t)message);
}
int sys_recv(pid_t from,Message *message){
  return kernel_call_asm(NR_K_RECV,(u32_t)from,(u32_t)message);
}
