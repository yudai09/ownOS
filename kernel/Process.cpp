#include"kcom.h"
#include"Process.h"
/*#include"Timer.h"
#include"asmfunc.h"
*/
/*
extern "C" {void taskSys();}
extern "C" {void taskIdle();}
*/

void mm_struct::init(){
  head = (mm_region *)kmalloc(sizeof(mm_region));
  head->next = NULL;
}
mm_struct::mm_struct(){
  init();
}
void mm_struct::add_region(u32_t *begin,u32_t size,u32_t flags){
  mm_region *p = head;
  while(p->next!=NULL){
    p=p->next;
  }
  mm_region *new_region = (mm_region *)kmalloc(sizeof(mm_region));
  p->next = new_region;
  new_region->set_region(begin,size,flags);
}
void mm_struct::add_region(const mm_region *region){
  mm_region *p = head;
  while(p->next!=NULL){
    p=p->next;
  }
  mm_region *new_region = (mm_region *)kmalloc(sizeof(mm_region));
  p->next = new_region;
  *new_region=*region;
}

Proc::Proc()
  :id(0),
   next(NULL),
   status(TASK_INIT),
   recvQ(Ipc::invMessage),
   sendQ(Ipc::invMessage)
{
  prior=PManager::Papp;
  //まだこの段階ではKMALLOCが使えない
  //  mm = (mm_struct *)kmalloc(sizeof(mm_struct));
}
PManager::PManager()
{
  pCurrent=NULL;
  pNext=NULL;
  for(int i=0;i<nr_Level;i++){
    pHead[i]=NULL;
    interval[i]=i;
  }
}
//default level que
void PManager::enque(class Proc *proc,u8_t level)
{
  Proc **pp;

  switch(proc->status){
  case Proc::TASK_STOPPED :
    break;//正常
  case Proc::TASK_RUNNING :
    warn("enque running process");
    return;//何もしないで終了
  default:
    panic("enque wrong process");
    //never reach here
    break;
  }
  
  if(level==0){
    pp=&pHead[proc->prior];
  }else{
    if(level<0||level>nr_Level){
      panic("enque(): wrong level");
    }
    pp=&pHead[level];
  }
  while((*pp)!=NULL){
    pp=&((*pp)->next);
  }
  proc->next=NULL;
  *pp=proc;

  proc->status = Proc::TASK_RUNNING;
}

void PManager::deque(class Proc *proc)
{
  
  switch(proc->status){
  case Proc::TASK_RUNNING :
    break;//正常
  case Proc::TASK_STOPPED :
    warn("denque stopped process");
    return;//何もしないで終了
  default:
    panic("enque wrong process");
    //never reach here
    break;
  }

  for(int i=0;i<nr_Level;i++){
    Proc **pp=&pHead[i];
    while(*pp!=NULL){
      if(*pp==proc){
	proc->status = Proc::TASK_STOPPED;
	*pp=(*pp)->next;
	//don't delete proc;
	return;
      }
      pp=&((*pp)->next);
    }
  }
  kprintf("deque() cannot find process \n");
}

Proc *PManager::getProc(pid_t id){
  return ProcL.findProc(id);
}
Proc *PManager::allocProc(){
  return ProcL.get();
}
extern "C" {void kernel2user(void);}

Proc *PManager::makeSysProc()
{
  Proc *pro=allocProc();
  entry_t *pdir=(entry_t *)pFAllocator.allocChunk(1);

  pro->mm = (mm_struct *)kmalloc(sizeof(mm_struct));
  pro->mm->init();
  pro->mm->cr3=(u32_t)pdir;
  u32_t *stack_begin,*stack_end;
  stack_begin = (u32_t *)(0x40000000-0x1000);
  stack_end   = (u32_t *)0x40000000;
  pro->mm->add_region(stack_begin,((u32_t)stack_end-(u32_t)stack_begin),
		      mm_region::READABLE | mm_region::WRITABLE | mm_region::SYSTEM);

  pro->status=Proc::TASK_STOPPED;
  pro->prior=Psys;

    //  varMem.editEntry(pdir,(u32_t)varMem.kPtables[0],VarMem::systempage);
  varMem.copyKernelDir(pdir);
  //void enableSpace(u32_t *virAddr,u32_t size,entry_t *pdir,const u16_t type);
  varMem.enableSpace((u32_t *)(0x40000000-0x1000),0x1000,(entry_t *)pro->mm->cr3,VarMem::systempage);//stack

  pro->kernel_info.ss  = (reg_t)Desc::KDSeg;
  pro->kernel_info.stack_top = (reg_t)pFAllocator.allocChunk(1)+0x1000-sizeof(void *);
  //  kprintf("stack addr %x \n",pro->kernel_info.esp);
  pro->kernel_info.cs  = (reg_t)Desc::KCSeg;
  pro->kernel_info.ret_from  = (reg_t)kernel2user;

  return pro;
}
