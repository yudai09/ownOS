#ifndef IPC_H_
#define IPC_H_
#include"com.h"

namespace Ipc{
  extern Message invMessage;  
  int send(pid_t id,Message &mess);
  int recv(pid_t id,Message &mess);

};

#endif
