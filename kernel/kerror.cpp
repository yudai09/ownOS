/*
 * kerror.c
 *
 *  Created on: 2010/05/09
 *      Author: yudai
 *      カーネル内で起こるエラーを処理する関数群
 */
#include"kcom.h"
#include"KGlobal.h"
#include"kerror.h"
void panic(const char *str){
  kprintf("PANIC: %s \n",str);
  for(;;);
}
void warn(const char *str){
  kprintf("WARN: %s \n",str);
}


