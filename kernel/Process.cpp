#include"kcom.h"
#include"KGlobal.h"
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
  new_region->next=NULL;
}

Proc::Proc()
  :id(0),
   next(NULL),
   status(TASK_INIT),
   recvQ(Ipc::invMessage),
   sendQ(Ipc::invMessage),
   recv_anyQ(Ipc::invMessage)
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

static Proc *pro;

static void register_stack(u32_t begin,u32_t end,bool is_user){
  u32_t size =  end-begin;

  if(is_user){
    pro->mm->add_region((u32_t *)begin,size,
                        mm_region::READABLE | mm_region::WRITABLE);
    varMem.enableSpace((u32_t *)begin,size,(entry_t *)pro->mm->cr3,VarMem::userpage);
  }else{
    pro->mm->add_region((u32_t *)begin,size,
                        mm_region::READABLE | mm_region::WRITABLE| mm_region::SYSTEM);
    varMem.enableSpace((u32_t *)begin,size,(entry_t *)pro->mm->cr3,VarMem::systempage);
  }
}
Proc *PManager::makeSysProc()
{
  entry_t *pdir;

  pro=(Proc *)allocProc();
  pdir=(entry_t *)pFAllocator.allocChunk(1);

  pro->mm = (mm_struct *)kmalloc(sizeof(mm_struct));
  pro->mm->init();
  pro->mm->cr3=(u32_t)pdir;

  pro->status=Proc::TASK_STOPPED;
  pro->prior=Psys;

  varMem.copyKernelDir(pdir);

  // register_stack(0x40000000-0x2000,0x40000000,true);
  // register_stack(0x80000000-0x2000,0x80000000,true);
  register_stack(VarMem::sys_stack_begin_init,VarMem::sys_stack_end,true);
  register_stack(VarMem::user_stack_begin_init,VarMem::user_stack_end,true);

  pro->kernel_info.ss  = (reg_t)Desc::KDSeg;
  pro->kernel_info.stack_top = (reg_t)pFAllocator.allocChunk(1)+0x1000-sizeof(void *);
  pro->kernel_info.cs  = (reg_t)Desc::KCSeg;
  pro->kernel_info.ret_from  = (reg_t)kernel2user;

  return pro;
}
