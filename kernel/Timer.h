#ifndef TIMER_H_
#define TIMER_H_

#include"com.h"

void timerHandler(void *args);

class Timer{
 public:
  const static u16_t interval=10;//割り込みの間隔
  const static u16_t nrEntry=0x20;
  enum type{INVALID,WatchDog,TaskSwitch,Wakeup};

  struct entry{
    int  clocks;//残りクロック
    u16_t type;
    u32_t pr_id;
    entry *next;
  };
  entry *head;//一番残り時間の小さいタイマー
  //  Lessor<entry,nrEntry> entryMan;

 public:
  bool quantum_exceeded;
  //プロセスがクオンタイムを使いきったか、もしくはプロセススイッチを発生させるためのフラグ
  u8_t msecond;//マイクロ秒をカウント
  u32_t second;//秒をカウント

  Timer();
  void set(int clocks,type t,u32_t pr_id);
  void passClocks(u16_t clocks);
  void reset_quantum(int clocks,u32_t pr_id);
  int isTimersEmpty(void);
  friend void timerHandler();
};

#endif
