#ifndef _TASK_H
#define _TASK_H

enum PIDS{
  PID_INVALID = -2,
  PID_KERNEL = -1,
  PID_INIT   = 1,
  PID_SYSTASK = 2,
  PID_PMAN,
  PID_IDLE,
  PID_ANY,//IPC use
};


extern "C" {void schedule();}
extern "C" {void kernel2user();}
extern "C" {void taskInit();}
void sleep(int pid,int millisec);
void sleep(int millisec=-1);
void sleep(void (*callback)(void));
void wakeup(int pid);
void wakeup();
void wait_interrupt(int irq);
void notify(int irq);
#endif
