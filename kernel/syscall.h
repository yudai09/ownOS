#ifndef _SYSCALL_H_
#define _SYSCALL_H_

enum syscall{
  NR_WAIT_INTERRUPT
};

int sys_fork();
int sys_recv(pid_t from,Message *message);
int sys_send(pid_t to,Message *message);
int sys_wait_interrupt(int irq);
//int syscall(u32_t nr,u32_t arg1,u32_t arg2,int arg3);

#endif
