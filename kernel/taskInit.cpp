#include"kcom.h"
#include"VarMem.h"
#include"task.h"
#include"syscall.h"
#include"ata.h"
#include"Ext2.h"
#include"elf_loader.h"
//#include"IDEDriver.h"
//Never sleep
extern "C" {void taskInit();}
#define IRQ_PRIMARY   14
#define IRQ_SECONDARY 15
int sys_fork();
void child(u32_t pid_parent){
  Message message;
  kprintf("I'm child \n");
  message.m1.p1=111;
  sys_send(pid_parent,&message);
  
  Ata *atadev = new Ata("hdd");
  Ext2 ext2_fs(atadev);

  kvector<kstring> path(1);
  path[0] = "grep";

  if(!ext2_fs.set_current_path(path)){
    kprintf("cannot find file");
  }else{
    u8_t *buffer = new u8_t[ext2_fs.blocksize()];
    ext2_fs.read_cn_block(14*0x400,buffer);
    load_elf_executable(path,ext2_fs);

  }
  //is he alive?
  while(1){
    for(int i=0;i<10000000;i++);
    kprintf("c");
  }
}
class Test{
public:
  Test(){
    kprintf("const test \n");
  }
};
void parent(u32_t pid_child){
  Message message;
  kprintf("I'm parent \n");
  
  sys_recv(pid_child,&message);
  kprintf("Message content = %d \n",message.m1.p1);
  while(1){
    for(int i=0;i<10000000;i++);
    kprintf("p");
  }
}

void taskInit(void){
  int y=0x22;
  u32_t *addr =  varMem.vir2phy((u32_t *)&y);
  kprintf("taskIinit %x %x \n",&y,addr);
  u32_t parent_pid = pManager.pCurrent->id;
  /*最後はアイドルタスクになる*/
  int *x = (int *)0x2000000;
  *x=1;
  int pid=sys_fork();
  if(!pid){
    child(parent_pid);
  }else{
    parent(pid);
  }
}
