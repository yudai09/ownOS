/*
 * irq.h
 *
 *  Created on: 2010/05/09
 *      Author: yudai
 */

#ifndef IRQ_H_
#define IRQ_H_

#include "kcom.h"

void notify(int irq);//def at task.cpp

template <int nr_irq>
void Irq_notify(void *){
  kprintf("Irq_template %d \n",nr_irq);
  notify(nr_irq);
}

extern "C" {void invalid(void *);}
extern "C" {void ignore(void *);}
extern "C" {void divide_exception(void *);}
extern "C" {void debuggingInterrupt(void *);}
extern "C" {void outofbundsException(void *);}
extern "C" {void invalidInstruction(void *);}
extern "C" {void noSegmentExeption(void *);}
extern "C" {void stackException(void *);}
extern "C" {void generalException(void *);}
extern "C" {void pageFault(void *);}
extern "C" {void kernelCall(void *);}
#endif /* IRQ_H_ */
