#include "kcom.h"
#include "task.h"
#include "Process.h"
#include "Timer.h"
#include "asmfunc.h"
#include "Descriptor.h"
#include "KGlobal.h"

static bool sched_req=false;

void init_tasks()
{
  /*タスクの実行はinitタスクから開始
   全てのタスクはこのinitタスクの子プロセスとなるようにする
  */
  Proc *pinit=pManager.makeSysProc();
  Proc *pkernel=pManager.makeSysProc();

  pkernel->id=PID_KERNEL;
  pinit->id=PID_INIT;

  pManager.enque(pinit);
  pManager.pCurrent=pinit;

  kprintf("init TSS.esp %x \n",pinit->kernel_info.stack_top);
  tss.setFrame2TSS(pinit->kernel_info);

  load_cr3_asm(pinit->mm->cr3);
  timer.reset_quantum(pManager.interval[0],pinit->id);

  taskswitch_asm(Desc::TCSeg|1,Desc::TDSeg|1,(u32_t)taskInit,0x40000000-0x20);
}
/*void print_context(const Proc *pro){
  stack_frame_s *frame =(stack_frame_s *)((u32_t)pro->kernel_info.stack_top-sizeof(stack_frame_s));
  //kprintf("print_context frame %x,esp %x eip %x flags %x \n",frame,frame->sp,frame->pc,frame->psw);
  }*/
/*
現在のスタックポインタの値がなんであろうと
ユーザ空間へ帰還するための関数
spの値をコンテキスト情報の先頭に設定し、ret2user(@isr.nas)
を呼ぶことで可能になる
*/

void after_exec(u32_t ret_addr){
  Proc *current = pManager.pCurrent;
  stack_frame_s *frame_curr =(stack_frame_s *)((u32_t)current->kernel_info.stack_top-sizeof(stack_frame_s));
  frame_curr->pc = (reg_t)ret_addr;
}
void ret_to_user(void){
  Proc *current = pManager.pCurrent;
  stack_frame_s *frame_curr =(stack_frame_s *)((u32_t)current->kernel_info.stack_top-sizeof(stack_frame_s));
  u32_t __dummy;
  asm volatile("mov %1,%%esp\n\t"
	       "jmp ret2user \n\t"
	       : "=r" (__dummy): "r" (frame_curr));
}
pid_t ret_from_fork(void){
  kprintf("ret_from_fork \n");
  Proc *current = pManager.pCurrent;
  stack_frame_s *frame =(stack_frame_s *)((u32_t)current->kernel_info.stack_top-sizeof(stack_frame_s));
  frame->ax = 0;//返り値
  /*スタックに戻りアドレスはつまれていないのでret_to_userを呼び出す必要がある*/
  ret_to_user();
}

void kernel2user(void){
  /*今のところ何もしない*/
  //  ret_to_user();
}

bool swap_swap;
void (*func)();

void schedule()
{
  if(!sched_req && !timer.quantum_exceeded){
    //    next_sf=pManager.pCurrent->stk_top();
    return ;
  }

  sched_req=timer.quantum_exceeded=false;
  u8_t nr_Level=pManager.nr_Level;

  for(int i=0;i<nr_Level;i++){
    Proc **pp=&(pManager.pHead[i]);
    if(*(pp)!=NULL){

      Proc *p_curr=*pp;
      Proc *p_prev=pManager.pCurrent;
      u8_t nlevel=(i<nr_Level-1)? i+1 : nr_Level-1;

      //deque
      (*pp)->status = Proc::TASK_STOPPED;
      *pp=(*pp)->next;
      //enque
      pManager.enque(p_curr,nlevel);
      timer.reset_quantum(pManager.interval[nlevel],p_curr->id);
      swap_swap = false;
      func = (void (*)())p_curr->kernel_info.ret_from;

      if(p_curr != p_prev){
	pManager.pCurrent=p_curr;
	pManager.pPrev=p_prev;
        tss.setFrame2TSS(p_curr->kernel_info);
	/*デフォルトの設定 swap_espよりも下だとだめ*/
        p_curr->kernel_info.ret_from = (reg_t) kernel2user;
	/*メモリ空間の切り替え*/
        load_cr3_asm(p_curr->mm->cr3);
        /*カーネルスタックを切り替え*/
        swap_esp(p_prev->kernel_info.sp,p_curr->kernel_info.sp);
	swap_swap = true;
      }
      /*スタックが切り替わった後はローカル変数へのアクセス等を禁止
       関数呼び出しも禁止*/
      if(swap_swap){
	func();
      }
      ret_to_user();
    }
  }
}
/* カーネルからユーザモードに戻るときに
 * 再スケジュールされるようにフラグを立てる
 */
void reschedule(){
  sched_req=true;
}
void clone_mm(const mm_struct *src_mm,mm_struct *dest_mm){
  entry_t *pdir=(entry_t *)pFAllocator.allocChunk(1);
  dest_mm->cr3 = (u32_t)pdir;
  varMem.copyKernelDir(pdir);
  mm_region *p = src_mm->head;
  while(p->next != NULL){
    p =  p->next;
    dest_mm->add_region(p);
    if(p->flags & mm_region::SYSTEM){
      //メモリ空間を有功にする（物理領域を割り当ててアクセス可能にする）
      varMem.enableSpace((u32_t *)p->begin,p->size,(entry_t *)dest_mm->cr3,VarMem::systempage);
      //4k単位でメモリの内容をコピーする
      kprintf("copy entry from %x size %x \n",p->begin,p->size);
      for(u32_t addr = (u32_t)p->begin; addr <= (u32_t)p->begin+(u32_t)p->size-0x1000;addr+=0x1000){
        //      for(u32_t *addr = (u32_t)p->begin; addr <= p->begin+p->size ;addr+=0x1000){
      	u32_t *phyAddr = varMem.vir2phy((u32_t *)addr,(entry_t *)dest_mm->cr3);//物理アドレスに変換
        memcpy(phyAddr,(u32_t *)addr,0x1000);
    //	return;
      }
    }else{
      kprintf("clone_mm: strange %x %x \n",p->flags,p->begin);
    }
  }
}
void copy_process(const Proc *src,Proc *dest){
  dest->prior = src->prior;
  dest->status = Proc::TASK_STOPPED;
  dest->mm = (mm_struct *)kmalloc(sizeof(mm_struct));
  dest->mm->init();
  clone_mm(src->mm,dest->mm);
}

void setup_stack(const Proc *current,Proc *clone){

  /*コンテキスト情報をコピーする処理 memcpyでもいい*/
  clone->kernel_info.stack_top = (reg_t)pFAllocator.allocChunk(1)+0x1000-4;//0x1000-4;

  stack_frame_s *frame_curr =(stack_frame_s *)((u32_t)current->kernel_info.stack_top-sizeof(stack_frame_s));
  stack_frame_s *frame_clone =(stack_frame_s *)((u32_t)clone->kernel_info.stack_top-sizeof(stack_frame_s));

  *frame_clone = *frame_curr;
  /*cloneの方が起床し時のスタックポインタの値を設定*/
  clone->kernel_info.sp = (reg_t)frame_clone;
}

pid_t fork(void)
{
  Proc *current = pManager.pCurrent;
  Proc *clone   = pManager.allocProc();
  clone->id = pManager.pid_gen();
  copy_process(current,clone);

  /*cloneが始めてスケジュールされる時のための設定*/
  clone->kernel_info = current->kernel_info;
  clone->kernel_info.ret_from = (reg_t)ret_from_fork;

  setup_stack(current,clone);
  wakeup(clone->id);
  return clone->id;
}
void sleep(void (*callback)(void)){
  kprintf("set call back \n");
  pManager.pCurrent->kernel_info.ret_from = (reg_t)callback;
  sleep(pManager.pCurrent->id,-1);
}
void sleep(int pid,int millisec)
{
  Proc *proc;
  
  if((proc=pManager.getProc(pid))==NULL){
    panic("sleep()::cannot find process");
    return;
  }
  //ランキューから除外
  pManager.deque(proc);
  //タイマーをセットして就寝
  if(millisec>0){
    timer.set(millisec /Timer::interval,Timer::Wakeup,proc->id);
  }

  reschedule();
}

void sleep(int millisec)
{
  sleep(pManager.pCurrent->id,millisec);
}

// プロセスの状態をWaitingにする
// タイマーがセットされている場合はそれを削除する
void wakeup(int pid)
{
  kprintf("wakeup %d \n",pid);
  Proc *proc;
  if((proc=pManager.getProc(pid))==NULL){
    panic("sleep()::cannot find process ");
    return;
  }
  pManager.enque(proc);
  reschedule();
}
void wakeup()
{
  wakeup(pManager.pCurrent->id);
}

struct entry{
  int irq;
  pid_t pid;
  entry(){}
  entry(int irq,pid_t pid){
    this->irq=irq;
    this->pid=pid;
  }
  bool operator==(const entry &rh){
    return this->irq==rh.irq ;
  }
};

entry invalid_entry(-1,PID_INVALID);
Que<entry,64> irqQue(invalid_entry);


void wait_interrupt(int irq)
{
  kprintf("wait_interrupt %d %d \n",irq,pManager.pCurrent->id);
  entry aentry(irq,pManager.pCurrent->id);
  irqQue.enque(aentry);
  sleep();
}
void notify(int irq){

  entry e=irqQue.deque(entry(entry(irq,0)));
  if(e==invalid_entry)
    return;
  kprintf("notify %d %d \n",e.irq,e.pid);
  wakeup(e.pid);
}
