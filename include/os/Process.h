#ifndef PROCESS_H_
#define PROCESS_H_
#include"kcom.h"
#include"Ipc.h"

class PManager;

struct mm_region{
private:

public:
  //最終的には下の３つのメンバーはprivateにしたい
  u32_t flags;
  u32_t *begin;//物理アドレス
  u32_t size;
  static const u32_t SYSTEM = 0x1;
  static const u32_t READABLE = 0x2;
  static const u32_t WRITABLE = 0x4;
  static const u32_t EXECUTABLE =0x8;
  mm_region *next;
  void set_region(u32_t *begin,u32_t size,u32_t flags,mm_region *next=NULL){
    this->begin = begin;
    this->size = size;
    this->flags = flags;
    this->next = next;
  }
};

class mm_struct{
 public:
  u32_t cr3;
  mm_region *head;//centinel node
  mm_struct();
  void init();
  void add_region(u32_t *begin,u32_t size,u32_t flags);
  void add_region(const mm_region *region);
};

struct kernel_info_s{
  reg_t stack_top;
  reg_t ss;
  reg_t sp;
  //  reg_t eip;
  reg_t ret_from;
  reg_t cs;
};
class Proc{
  const static u16_t nrMessage=32;//num of available message in que
 public:
  enum STATUS{TASK_INIT,TASK_SLEEP,TASK_RUNNING,TASK_STOPPED};
  pid_t id;
  Proc *next;
  //  u32_t cr3;
  u8_t prior;
  u8_t status;
  mm_struct *mm;
  struct kernel_info_s kernel_info;
  //struct stack_frame_s *stkf;
  //    MessageInfo send_any;//使うかどうかわからん
  //  Message recv_any;

  Que <Message,nrMessage> recvQ;
  Que <Message,nrMessage> sendQ;
  Que <Message,nrMessage> recv_anyQ;
  Proc();
  void init();
  /*  inline u32_t *stk_top(){
    return &(stkf->gs);
    }*/
  
  friend class PManager;
};
//超適当
class PID_GEN{
  u32_t num;
 public:
  PID_GEN(){
    num = 100;
  }
  int operator()(){
    num++;
    return num-1;
  }
};

class PManager{

 public:

  static const u16_t nr_Proc=32;
  static const u8_t  nr_Level=4;

  u8_t  interval[nr_Level];//={4,3,2,1};
  Proc *pPrev;
  Proc *pCurrent;//currentProcess
  Proc *pNext;//nextprocess to schedule
  Proc *pHead[nr_Level];//pointer to head process of each level

  PID_GEN pid_gen;

  class PLOCL :public Lessor<Proc,nr_Proc>{
  public:
    Proc *findProc(pid_t id){
      for(int i=0;i<nr_Proc;i++){
  	if(!isFree[i]){
  	  if(room[i].id==id)
  	    return &room[i];
  	}
      }
      return NULL;
    }
  }ProcL;
  Proc *getProc(pid_t id);
  Proc *allocProc();
  PManager();
  enum priority{Pker,Psys,Papp};  
  void enque(Proc *p,u8_t level=0);
  void deque(Proc *p);

  void lower(Proc *p);
  Proc *makeSysProc();

};



#endif
