/*
 * PFAllocator.h
 *
 *  Created on: 2010/05/09
 *      Author: yudai
 */

#ifndef PFALLOCATOR_H_
#define PFALLOCATOR_H_
#include"com.h"

class PFAllocator {
	static const u16_t MaxNumSeg=128;

public:
	static const u32_t ChunkSize=0x1000;
	struct Seg{
		u8_t	free;
		u32_t  begin;
		u32_t  blocks;//replace
		Seg *np;//pointer
	};

	Seg segs[MaxNumSeg];

	Seg *seg_free;
	Seg *seg_used;
 private:
	Seg *getSeg();
	void freeSeg(Seg *seg);
 public:
	PFAllocator();
    /*以下は初期化時のみ使う*/
 public:
	void addChunk(Seg *seg);
	void addChunk(u32_t begin,u32_t end);
	void usedChunk(Seg *seg);
	u32_t *removeChunk(u32_t bAddr_rm,u32_t eAddr_rm);
    /*以下が本来の処理*/
 public:
	u32_t *allocChunk(u16_t seriesNum);
	void freeChunk(u32_t *addr);

};

#endif /* PFALLOCATOR_H_ */
