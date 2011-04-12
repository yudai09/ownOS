#include"kcom.h"
#include"VarMem.h"
#include"task.h"
#include"syscall.h"
#include"ata.h"
#include"Ext2.h"
//#include"elf_loader.h"
#include "exec.h"
#include"FileSystem.h"
#include"KGlobal.h"
#include"printf.h"
#include"vga.h"


//#include"IDEDriver.h"
//Never sleep
extern "C" {void taskInit();}
#define IRQ_PRIMARY   14
#define IRQ_SECONDARY 15
int sys_fork();
int sys_esec();
void child(u32_t pid_parent){
  Message message;
  kprintf("I'm child \n");

  message.m1.p1=111;
  sys_send(pid_parent,&message);

  Ata *atadev = new Ata((char *)"hdd");
  //  Ext2 ext2_fs(atadev);
  Ext2 *root_fs = new Ext2(atadev);
  kvector<kstring> path(1);
  path[0] = "executable";

  // if(!root_fs->set_current_path(path)){
  //   kprintf("cannot find file");
  // }else{
  //   u8_t *buffer = new u8_t[root_fs->blocksize()];
  //   root_fs->read_cn_block(14*0x400,buffer);
  //   load_elf_executable(path,*root_fs);
  // }
  
  //  FileSystem fs(root_fs);
  fs = new FileSystem(root_fs);
  kprintf("addr %x \n",&fs);
  //  File *file=fs->fopen(kstring((char *)"/executable.elf"));


  File *file=fs->fopen(kstring((char *)"/user/user.elf"));
  kprintf("file %x \n",file);

  if(file->is_valid){
    if(sys_exec((char *)"/user/user.elf"))
      kprintf("exec sucess \n");
    else
      kprintf("exec failed \n");
  }else{
    kprintf("cannot open file \n");
  }
  //画面を大きく
  Vram::init_vga();
  init_vx_printf();

  //is he alive?

  kprintf("roop \n");
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
  int pid=sys_fork();
  if(!pid){
    child(parent_pid);
  }else{
    parent(pid);
  }
}
