/*
 * type_1.h
 *
 *  Created on: 2009/11/18
 *      Author: yudai
  */

#ifndef TYPE_HPP_
#define TYPE_HPP_
/*汎用的な宣言*/
#include "kerror.h"

#define NULL 0

typedef unsigned int reg_t;
typedef unsigned short reg16_t;

typedef unsigned char  u8_t;
typedef unsigned short u16_t;
typedef unsigned int   u32_t;
typedef unsigned long long int u64_t;

typedef char  s8_t;
typedef short s16_t;
typedef int s32_t;
typedef long long int s64_t;

typedef unsigned int entry_t;

typedef int pid_t;



struct TSS32 {
        unsigned int backlink, esp0, ss0, esp1, ss1, esp2, ss2, cr3;
        unsigned int eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
        unsigned int es, cs, ss, ds, fs, gs;
        unsigned int ldtr, iomap;
};

struct seg_desc_s{
  u16_t limit_low,base_low;
  u8_t  base_mid,access_right;
  u8_t limit_high,base_high;
};

struct gate_desc_s{
  u16_t offset_low;
  u16_t selector;
  u8_t count;
  u8_t access_right;
  u16_t offset_high;
};


// struct stack_frame_s {           /* proc_ptr points here */
//   reg_t  isFree;                 /* this frame is free or not */
//   reg_t RetFromShedule;          /* retaddr from shedule() $B>e=q$-$5$l$k$?$a$@$1$NNN0h(B */
//   reg_t gs;                     /* last item pushed by save */
//   reg_t fs;                     /*  ^ */

//   reg_t es;                     /*  | */
//   reg_t ds;                     /*  | */
//   reg_t di;			/* di through cx are not accessed in C */
//   reg_t si;			/* order is to match pusha/popa */
//   reg_t bp;			/* bp */
//   reg_t st;			/* hole for another copy of sp */
//   reg_t bx;                     /*  | */
//   reg_t dx;                     /*  | */
//   reg_t cx;                     /*  | */
//   reg_t ax;			/* ax and above are all pushed by save */

//   reg_t pc;			/*  ^  last item pushed by interrupt */
//   reg_t cs;                     /*  | */
//   reg_t psw;                    /*  | */
//   reg_t sp;                     /*  | */
//   reg_t ss;                     /* these are pushed by CPU during interrupt */
// };
struct stack_frame_s {           /* proc_ptr points here */
  reg_t gs;                     /* last item pushed by save */
  reg_t fs;                     /*  ^ */

  reg_t es;                     /*  | */
  reg_t ds;                     /*  | */
  reg_t di;			/* di through cx are not accessed in C */
  reg_t si;			/* order is to match pusha/popa */
  reg_t bp;			/* bp */
  reg_t st;			/* hole for another copy of sp */
  reg_t bx;                     /*  | */
  reg_t dx;                     /*  | */
  reg_t cx;                     /*  | */
  reg_t ax;			/* ax and above are all pushed by save */

  reg_t pc;			/*  ^  last item pushed by interrupt */
  reg_t cs;                     /*  | */
  reg_t psw;                    /*  | */
  reg_t sp;                     /*  | */
  reg_t ss;                     /* these are pushed by CPU during interrupt */
};


class Message{
public:
  pid_t id;
  u16_t type;
  bool isValid;
  union {//body
    struct {
      u32_t p1;
      u32_t p2;
      u32_t p3;
    }m1;
    struct {
      u32_t p1;
    }m2;
  };
  Message(){
    isValid=true;
  }
  Message(pid_t id){
    this->id=id;
    isValid=true;
  }
  Message(bool isValid){
    this->isValid=isValid;
  }
  bool operator==(Message &rh){
    return id==rh.id;
  }
};

//配列の要素をgetしたりfreeしたりするテンプレートクラス
template<class T,int N> class Lessor{//管理人(配列の要素の型、要素数)
public:
  T room[N];//T型の部屋がN個
  bool isFree[N];//該当する部屋が空きか？
  Lessor(){
    for(int i=0;i<N;i++){
      isFree[i]=true;
    }
  }
  T *get(){//部屋を借りる
    for(int i=0;i<N;i++){
      if(isFree[i]){
    	  isFree[i]=false;
    	  return &(room[i]);
      }
    }
    panic("Lessor cannot get free room\n");
    return NULL;//(NULL)
  }
  void free(const T *a){//部屋を空ける
    for(int i=0;i<N;i++){
      if(a==room+i){
	isFree[i]=true;
	return;
      }
    }
  }
};
 

template < class T,int N >
class Que{
  const static  int nr_ent=N;
  class Entry{
    bool sentinel;
  public:
    Entry *next;
    Entry *prev;
    T content;
    Entry(){
      next=NULL;
    }
    void setSentinel(bool sentinel){
      this->sentinel=sentinel;
    }
  };
  T invalid;
  Entry *head;
  Entry *tail;
  int num;

public:
  Lessor<Entry,nr_ent+2> entries;
  Que(T invalid)
  {
    this->invalid=invalid;
    head=entries.get();
    tail=entries.get();
    head->setSentinel(true);
    tail->setSentinel(true);
    head->next=tail;tail->prev=head;
    head->prev=tail->next=NULL;
    num=0;
  }
  void enque(const T &t){
    if(num>nr_ent){
      //      error();
      panic("Que is full");
      return;
    }
    Entry *nHead=head->next;
    Entry *aent=entries.get();
    aent->content=t;
    aent->next=nHead;
    aent->prev=head;
    head->next=aent;
    nHead->prev=aent;
    num++;
  }
  T deque(){
    if(num==0){
      return invalid;
    }
    Entry *pTail=tail->prev;
    Entry *ppTail=pTail->prev;
    T ret=pTail->content;
    ppTail->next=tail;
    tail->prev=ppTail;
    entries.free(pTail);
    num--;
    return ret;    
  }
  T deque(T a){
    Entry *curr=head->next;
    for(int i=0;i<num;i++){
      if(curr->content==a){//T.==operator
	Entry *pCurr=curr->prev;
	Entry *nCurr=curr->next;
	T ret=curr->content;
	pCurr->next=nCurr;
	nCurr->prev=pCurr;
	entries.free(curr);
	num--;
	return ret;
      }
      curr=curr->next;
    }
    return invalid;
  }
};


#endif /* TYPE_1_H_ */
