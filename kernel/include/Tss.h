/*
 * tss.h
 *
 *  Created on: 2009/11/19
 *      Author: yudai
 */

#ifndef TSS_H_
#define TSS_H_

#include"com.h"

#define numFrames 0x40

class Tss {
	struct TSS32 *tss_p;//唯一のTSS　特権レベルの下降時にスタックを切り替えるための設定を保持
	struct stack_frame_s *frames; //プロセス情報を保持
	void init_tssManager(void);
	void init_tss(void);
public:
	Tss();
	struct stack_frame_s *allocStackFrame(void);
	void freeStackFrame(struct stack_frame_s *p);
	void setFrame2TSS(struct stack_frame_s *sf);
};

#endif /* TSS_H_ */
