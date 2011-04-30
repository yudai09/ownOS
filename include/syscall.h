#ifndef SYSCALL_H
#define SYSCALL_H
#include<com.h>
#include<Ipc.h>

enum syscall{
  NR_WAIT_INTERRUPT,
  NR_WRITE,
  NR_READ,
};

int sys_recv(pid_t from,Message *message);
int sys_send(pid_t to,Message *message);

#endif

