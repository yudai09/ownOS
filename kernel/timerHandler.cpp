#include"kcom.h"
#include "Timer.h"
#include "task.h"
void timerHandler(void *args){
  timer.msecond+=Timer::interval;
  if(timer.msecond>1000){//秒数を数える
    timer.second++;
    timer.msecond=0;
  }
  timer.passClocks(1);

  Timer::entry *p=timer.head;
  while(p!=NULL){
    if((p)->clocks>0){
      break;
    }
    switch(p->type){
    case Timer::WatchDog:
      //まだ
      break;
    case Timer::TaskSwitch:
      timer.quantum_exceeded=true;
      break;
    case Timer::Wakeup:
      wakeup(p->pr_id);
      break;
    case Timer::INVALID:
    default:
      //error();
      break;
    }
    //    timer.entryMan.free(p);
    kfree(p);
    p=p->next;
  }
  timer.head=p;
}