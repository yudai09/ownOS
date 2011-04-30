#ifndef _TASK_H
#define _TASK_H

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
void after_exec(u32_t ret_addr);
#endif
