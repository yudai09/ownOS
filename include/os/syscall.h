#ifndef _SYSCALL_H_
#define _SYSCALL_H_
#include"Ipc.h"
enum syscall{
  NR_WAIT_INTERRUPT,
  NR_WRITE,
  NR_READ,
};
int sys_exec(char *file_path,enum priv p);
int sys_fork();
int sys_recv(pid_t from,Message *message);
int sys_send(pid_t to,Message *message);

#endif
