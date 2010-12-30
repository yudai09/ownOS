#include"KGlobal.h"
#include"Timer.h"

Timer::Timer()
  : head(NULL),
    quantum_exceeded(0),
    msecond(0),
    second(0)
{

}
void Timer::set(int clocks,type t,u32_t pr_id)
{
  //  entry *aEntry=entryMan.get();
  entry *aEntry = (entry *)kmalloc(sizeof(entry));
  if(aEntry==NULL){
    //error();
    return;
  }
  aEntry->clocks=clocks;
  aEntry->type=t;
  aEntry->pr_id=pr_id;
  entry **p=&head;
  while(1){
    if(*p==NULL){//末尾
      *p=aEntry;
      aEntry->next=NULL;
      return;
    }
    if((*p)->clocks > clocks){
      aEntry->next=(*p);
      *p=aEntry;
      return;
    }
    p=&((*p)->next);
  }
}
//各タイマーからクロックを引く
void Timer::passClocks(u16_t clocks)
{
  entry *p=head;
  while(p!=NULL){
    p->clocks-=clocks;
    p=p->next;
  }
}

//すべてのタスクスイッチタイマを削除し新たなタスクスイッチタイマを設定
void Timer::reset_quantum(int clocks,u32_t pr_id)
{
  entry **pp=&head;
  while(*pp!=NULL){
    entry **next=&((*pp)->next);
    if((*pp)->type==TaskSwitch){
      //      entryMan.free(*pp);//解放作業
      kfree(*pp);
      (*pp)=(*pp)->next;
    }else{
      pp=next;
    }
  }
  set(clocks,TaskSwitch,pr_id);
}

// void timerHandler(void *args){
//   //  kprintf("timerHandler\n");
//   timer.msecond+=Timer::interval;
//   if(timer.msecond>1000){//秒数を数える
//     timer.second++;
//     timer.msecond=0;
//   }
//   timer.passClocks(1);

//   Timer::entry *p=timer.head;
//   while(p!=NULL){
//     if((p)->clocks>0){
//       break;
//     }
//     switch(p->type){
//     case Timer::WatchDog:
//       //まだ
//       break;
//     case Timer::TaskSwitch:
//       timer.quantum_exceeded=true;
//       break;
//     case Timer::Wakeup:
//       wakeup(p->pr_id);
//       break;
//     case Timer::INVALID:
//     default:
//       //error();
//       break;
//     }
//     timer.entryMan.free(p);
//     p=p->next;
//   }
//   timer.head=p;
// }
