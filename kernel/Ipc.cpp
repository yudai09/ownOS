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
      return 0;
    }else{
      //recv ANY
      if(psent->recv_anyQ.deque().isValid){
        psent->sendQ.enque(mess);//enque
        kprintf("%d send -> %d\n(ANY)",myself->id,id);
        wakeup(id);
      }else{
        psent->sendQ.enque(mess);//enque
        kprintf("%d wait to send -> %d\n",myself->id,id);
        sleep();
      }
    }
    return 0;
  }

  void recv_later(){
    Message message;
    kprintf("recv_later \n");
    Proc *myself = pManager.pCurrent;
    message = myself->sendQ.deque();
    if(!message.isValid){
      panic("invalid message error at recv_later()");
    }
  }

  int recv(pid_t id,Message &mess){
    Proc *myself=pManager.pCurrent;
    Proc *precv=pManager.getProc(id);
    Message tmess;
    if(id==PID_ANY){
      tmess=myself->sendQ.deque();//deque
    }else{
      tmess=myself->sendQ.deque(Message(id));//deque
    }

    if(tmess.isValid){
      kprintf("%d recv <- %d\n",myself->id,id);
      kprintf("mp1 %d \n",mess.m1.p1);
      mess = tmess;//copy
      wakeup(id);
    }else{//wait a message
      if(id==PID_ANY){
        kprintf("will recv from Any process \n");
        myself->recv_anyQ.enque(mess);
      }else{
        precv->recvQ.enque(mess);//enque
      }
      kprintf("%d wait to recv <- %d\n",myself->id,id);
      sleep(recv_later);//set call back func
    }
  }
  Message invMessage(false);
};
