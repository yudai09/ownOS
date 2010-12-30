#include"kcom.h"
#include"VarMem.h"
#include"task.h"
#include"syscall.h"
#include"ata.h"
#include"Ext2.h"
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
  //IDEDriver *ide_driver;
  //ide_driver = new IDEDriver(IRQ_PRIMARY,IRQ_SECONDARY);
  //int controller, deviceNo;
  // if (!ide_driver->findDevice(IDEDriver::DEVICE_ATA, -1, &controller, &deviceNo)){
  //   kprintf("HD Not Found\n");
  //   for(;;);
  // }else{
  //   kprintf("HD Found (Success)\n");
  // }
  Ata *atadev = new Ata("hdd");
  u8_t buffer[100];

  atadev->read(0,buffer,100);
  for(int i=0;i<100;i++){
    kprintf("%x",buffer[i]);
  }
  set_ext2_root(atadev);
  //  ata.                          
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
