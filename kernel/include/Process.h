 /*
 * process.h
 *
 *  Created on: 2009/11/19
 *      Author: yudai
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include"com.h"
#include"ipc.h"
#include"BinaryStore.h"
#define MaxLevel  6

extern u32_t *next_stackframe_top;

class Process {

public:
	static const u8_t priority_kernel=0;
	static const u8_t priority_system=1;
	static const u8_t priority_app=3;
	static const u8_t priority_lowest=MaxLevel-1;



	class Proc{//この構造体のなかに配列などを加えるとProcManager中で大量のコピーが発生するので注意
	public:
		u32_t maxpriority;
		enum STATE{INVALID,DEAD,SLEEP,EXCUTABLE};
		STATE state;
		enum LOCK_STATE{UNLOCKED,LOCKED};
		LOCK_STATE lock_state;
		int id;
		//int level,index;
		Proc *myself;
		int used;

		u32_t cr3;
		//  int argument;//引数
		struct  stack_frame_s *stack_frame;
		MessageInfo recv_ANY;
		MessageInfo send_ANY;//使うかどうかわからん
		WaitQue recvQ;
		WaitQue sendQ;

		Proc()
		{
			id=0;
			//level=-1;
			//index=-1;
			used=0;
			state=Proc::DEAD;
			lock_state=UNLOCKED;
		}
		LOCK_STATE lock(){
			int result;
			asm volatile("cmpxchg %1, %2\n" : "=a"(result) : "r"(LOCKED), "m"(lock_state), "a"(UNLOCKED));
			return (LOCK_STATE)result;
		}
		void unlock(){
			lock_state=UNLOCKED;
		}

	};
	/*Queはポインターではなくコピーを所持しているのでProcessの変更を伝えるためには一度コピーしなおす必要がある
	 CurrentProcesssについても同様*/
	class PROCMAN{
	public:
		static const u16_t MaxTasks =32;
		Proc procs[MaxTasks];
		Proc *que[MaxLevel][MaxTasks];//Que
		int	 numProc_inQ[MaxLevel];
		Proc *allocProc();
		void freeProc(u32_t id);
		void append_tail(struct Proc *p,int level);
		void deletefromQ(int level,int nth);
		void deletefromQ(int pid);
		void lowerLevel(struct Proc *pro,int level,int nth);
		struct Proc *getProc(unsigned int ID);
		//void initProcManager(void);
	}processManager;

	class MessageBuffer{
	public :
		MessageInfo messages[PROCMAN::MaxTasks*3];
		int p;
		MessageBuffer(){
			p=0;
		}
		void push(MessageInfo message){
			if(p==PROCMAN::MaxTasks)
				error("Process::Proc too many message buffered");
			messages[p++]=message;
		}
	}Mbuff;


	Proc currentProcess;
	Process();
	void init_process();
	void initProcManager(void);
	Proc *runSystemProcess(u32_t eip,u16_t codedesc,u16_t datadesc,u32_t stack_begin,int pid);
	void sleep(int millisec);
	void sleep_kernel(int id,int millisec);
	void wakeup_kernel(int pr_id);
};


//void beginMultiTask(void);
extern "C" {void user_process(int myid);}
extern "C" {void user_process2(int myid);}
extern "C" {void user_process3(int myid);}
extern "C" {void idle_process(int myid);}
extern "C" {void schedule(void);}
extern "C" {void wakeup_kernel(int pr_id);}
/*この構造体はプロセス状態を保存するのに使う
プロセス状態PSWはプロセスの開始時に保存される
*/
#endif /* PROCESS_H_ */
