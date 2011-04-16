#ifndef IPC_H_
#define IPC_H_
#include"com.h"
class Message{
public:
  pid_t id;
  u16_t type;
  bool isValid;
  union {//body
    struct {
      u32_t p1;
      u32_t p2;
      u32_t p3;
    }m1;
    struct {
      u32_t p1;
    }m2;
  };
  Message(){
    isValid=true;
  }
  Message(pid_t id){
    this->id=id;
    isValid=true;
  }
  Message(bool isValid){
    this->isValid=isValid;
  }
  bool operator==(Message &rh){
    return id==rh.id;
  }
};

namespace Ipc{
  extern Message invMessage;  
  int send(pid_t id,Message &mess);
  int recv(pid_t id,Message &mess);

};

#endif
