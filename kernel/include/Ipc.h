/*
 * ipc.h
 *
 *  Created on: 2009/12/03
 *      Author: yudai
 */

#ifndef IPC_H_
#define IPC_H_
#include"ipc.h"
#include"com.h"


struct IPCmessage{//メッセージ本体
	enum MessageType{INVALID,ACK,NACK,SYSTEM_CALL,ERROR,REINCARNATE,OTHER};
	MessageType message_id;
	int length;//メッセージの大きさ
	u8_t body[0x400];//メッセージボディー
};

class MessageInfo{
public:
	//int id;
	enum TYPE{INVALID,SEND,RECV};
	TYPE type;
	int src;
	int dest;
	u32_t addr_phy;

	MessageInfo(){
		type=INVALID;
		addr_phy=0;
	}
	MessageInfo(TYPE type,int src,int dest,u32_t paddr){
		set(type,src,dest,paddr);
	}
	void set(TYPE t,int s,int d,u32_t paddr){
		type=t;
		src=s;
		dest=d;
		addr_phy=paddr;
	}
};

class Ipc {
public:
	Ipc();
	int send(int id,IPCmessage *buff);
	int recv(int id,IPCmessage *buff);
	int inform(int id);
	int finishReincarnate(MessageInfo *infos);
	//int notify(int id,IPCmessage *ipcmessage);
	//int notify_k(int id,IPCmessage *ipcmessage);
};




class WaitQue{
	static const u16_t MAXQ=32;
	MessageInfo datas[MAXQ];
	u32_t	head;
	u32_t	tail;
public:
	MessageInfo errorret;
	WaitQue()
	{
		errorret.set(MessageInfo::INVALID,0,0,0);//
		head=tail=0;//初期化
	};
	void dump(){
		head=tail=0;
	}
	int put_q(const MessageInfo data)
	{
		int nexthead=(head+1)%MAXQ;
		if(nexthead==tail){
			return -1;
		}
		datas[head]=data;
		head=nexthead;
	};
	MessageInfo get_q()
	{
		if(head==tail)
			return errorret;
		int tail_past=tail;
		tail=(tail+1)%MAXQ;
		return datas[tail_past];
	};
	MessageInfo get_q(int dest){
		MessageInfo ret=errorret;//初期化
		for(int i=tail;i!=head;i=(i+1)%MAXQ)
			if(datas[i].dest==dest){
				ret=datas[i];
				//printf("head %d \n",head);
				while(i!=head){
					int next_i=(i+1)%MAXQ;
					datas[i]=datas[next_i];
					i=next_i;
				}
				if(head==0)
					head=MAXQ-1;
				else
					head--;
				break;
			}
		return ret;
	}

};

#endif /* IPC_H_ */
