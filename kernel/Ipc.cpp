#include"kcom.h"
#include"Ipc.h"
#include"Process.h"
#include"task.h"
#include "KGlobal.h"
/*
超非効率なメッセージパッシング
enque->deque->copyの三回のコピーが全体で発生する
*/
namespace Ipc{
  
  int send(pid_t id,Message &mess){
    Proc *myself=pManager.pCurrent;
    Proc *psent=pManager.getProc(id);
    Message tmess=myself->recvQ.deque(Message(id));

    mess.id = myself->id;
    if(tmess.isValid){
      psent->sendQ.enque(mess);//enque
      kprintf("%d send -> %d\n",myself->id,id);
      wakeup(id);
    }else{
      psent->sendQ.enque(mess);//enque
      kprintf("%d wait to send -> %d\n",myself->id,id);
      sleep();
    }
    return 0;
  }
  Message *wait_message;
  void recv_later(){
    kprintf("recv_later \n");
    Proc *myself = pManager.pCurrent;
    *wait_message = myself->sendQ.deque(*wait_message);
    if(!wait_message->isValid){
      panic("invalid message error at recv_later()");
    }
  }
  int recv(pid_t id,Message &mess){
    Proc *myself=pManager.pCurrent;
    Proc *precv=pManager.getProc(id);
    Message tmess=myself->sendQ.deque(Message(id));//deque
    //Message tmess=myself->sendQ.deque();//deque
    kprintf("id = %d \n",tmess.id);
    if(tmess.isValid){
      kprintf("%d recv <- %d\n",myself->id,id);
      kprintf("mp1 %d \n",mess.m1.p1);
      mess = tmess;//copy
      wakeup(id);
    }else{
      precv->recvQ.enque(mess);//enque
      wait_message = &mess;
      wait_message->id = id;
      kprintf("%d wait to recv <- %d\n",myself->id,id);
      sleep(recv_later);//set call back func
    }
  }
  Message invMessage(false);
};
