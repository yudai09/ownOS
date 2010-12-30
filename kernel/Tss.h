/*
 * tss.h
 *
 *  Created on: 2009/11/19
 *      Author: yudai
 */

#ifndef TSS_H_
#define TSS_H_

#include"com.h"
#include "Process.h"


//#define numFrames 0x40
#define NRDESC 0x20
//class Tss:public Lessor<stack_frame_s,NRDESC>{
class Tss{

 struct TSS32 *tss_p;//唯一のTSS　特権レベルの下降時にスタックを切り替えるための設定を保持
 public:
  void initTSS(void);
  Tss();
  //	struct stack_frame_s *allocStackFrame(void);
  // void freeStackFrame(struct stack_frame_s *p);
  //void setFrame2TSS(struct stack_frame_s *sf);
  void setFrame2TSS(const struct kernel_info_s &ki);
};

#endif /* TSS_H_ */
