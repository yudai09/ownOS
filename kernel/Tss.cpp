/*
 * tss.cpp
 *
 *  Created on: 2009/11/19
 *      Author: yudai
 */

#include "KGlobal.h"
#include "Tss.h"


Tss::Tss(){
  
}
void Tss::initTSS(){
  tss_p=(TSS32 *)pFAllocator.allocChunk(1);//無駄遣いだけどアラインされた領域が必要
  tssaddr=(u32_t)tss_p;

  bzero((void *)tss_p,sizeof(struct TSS32));//ゼロで埋める
}
/*スタックは後ろからつまれていくので、あるスタックフレームをTSSのesp0に
設定するこの関数はesp0に引数のスタックフレームのポインター＋スタックフレーム
のサイズを設定する*/
/*
void Tss::setFrame2TSS(struct stack_frame_s *sf){
  tss_p->ss0=(reg_t)Desc::KDSeg;
  tss_p->esp0=(reg_t)sf+sizeof(struct stack_frame_s);//スタックは大きなアドレスからPUSHされるので
  }*/


void Tss::setFrame2TSS(const struct kernel_info_s &ki){
  tss_p->ss0 = ki.ss;
  tss_p->esp0 = ki.stack_top;
}


