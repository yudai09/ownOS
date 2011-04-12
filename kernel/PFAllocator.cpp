/*
 * PFAllocator.cpp
 *
 *  Created on: 2010/05/09
 *      Author: yudai
 *      ページフレーム単位での領域確保を行うクラス
 *      ４８６ではページフレームは４k（０x１０００）バイト
 */

#include "PFAllocator.h"
#include "kcom.h"

PFAllocator::PFAllocator()
{
  for(int i=0;i<MaxNumSeg;i++){
    segs[i].free=TRUE;
  }
  seg_free=(Seg *)NULL;
  seg_used=(Seg *)NULL;
}

/*
 * 空き領域を追加する関数
 * もし追加する領域が他の空き領域と連続した領域である場合は結合する
 * 空き領域はアドレス順に並ぶ
 * 結合する場合は追加する領域を表すsegを開放する
 * 手順１　挿入するところを決定する
 * 手順２　前後の領域と領域を結合できないか確認
 * 手順３　できない場合は挿入、できる場合は結合、不要なSEGを開放
 * */

void PFAllocator::addChunk(Seg *seg)
{
  //Seg **pp_seg=&seg_free;
  Seg **cseg=&seg_free;
  Seg **nseg;
  while(1){
    if(*cseg==NULL){
      *cseg=seg;
      return;
    }
    nseg=&(*cseg)->np;

    if(*nseg!=NULL){
      if((*cseg)->begin < seg->begin && seg->begin < (*nseg)->begin){
	//結合できるか
	if((*cseg)->begin+(*cseg)->blocks*ChunkSize==seg->begin){
	  (*cseg)->blocks+=seg->blocks;
	  freeSeg(seg);
	  if((*cseg)->begin+(*cseg)->blocks*ChunkSize==(*nseg)->begin){
	    (*cseg)->blocks+=(*nseg)->blocks;
	    freeSeg(*nseg);
	    //上の行と下の行を入れ替えることができない
	    //NSEGは&CSEG->NPなので
	    (*cseg)->np=(*nseg)->np;
	  }
	  return;
	}
	else if(seg->begin+seg->blocks*ChunkSize==(*nseg)->begin){
	  (*nseg)->blocks+=seg->blocks;
	  (*nseg)->begin=seg->begin;
	  freeSeg(seg);
	  return;
	}

	//結合できなかった
	seg->np=(*nseg);
	(*cseg)->np=seg;
	return;
      }
    }
    else{
      //結合できるか？
      if((*cseg)->begin+(*cseg)->blocks*ChunkSize==seg->begin){
	(*cseg)->blocks+=seg->blocks;
	freeSeg(seg);
	return;
      }
      (*cseg)->np=seg;
      return;
    }
    cseg=nseg;
  }
}
void PFAllocator::addChunk(u32_t begin,u32_t end)
{
  u32_t B=begin&~(ChunkSize-1);//round
  u32_t E=end&~(ChunkSize-1);//round
  u32_t block=(E-B)/ChunkSize;
  Seg *seg=getSeg();
  seg->begin=B;
  seg->blocks=block;
  seg->np=(Seg *)NULL;
  addChunk(seg);
}
/*allocChunkによって確保され、使用中の領域のリストを保存する関数
 * 構造はaddChunkに似ているが領域を結合しない
 * */
void PFAllocator::usedChunk(Seg *seg)
{
  Seg **cseg=&seg_used;
  Seg **nseg;
  while(1){
    if(*cseg==NULL){
      *cseg=seg;
      return;
    }
    nseg=&(*cseg)->np;

    if(*nseg!=NULL){
      if((*cseg)->begin < seg->begin && seg->begin < (*nseg)->begin){
	//結合できなかった
	seg->np=(*nseg);
	(*cseg)->np=seg;
	return;
      }
    }
    else{
      (*cseg)->np=seg;
      return;
    }
    cseg=nseg;
  }
}

/*
 * 空き領域の確保を行う関数
 *
 * */
u32_t * PFAllocator::allocChunk(u16_t seriesNum)
{
  Seg **pp_seg=&seg_free;
  Seg *useg=getSeg();//allocated seg
  while((*pp_seg)!=NULL){
    if((*pp_seg)->blocks >= seriesNum){
      useg->begin=(*pp_seg)->begin;
      useg->blocks=seriesNum;
      useg->np=NULL;
      (*pp_seg)->blocks-=seriesNum;
      (*pp_seg)->begin+=seriesNum*ChunkSize;
      if((*pp_seg)->blocks==0){//空の要素を削除
	freeSeg(*pp_seg);
	*pp_seg=(*pp_seg)->np;
      }
      //使用中の領域リストへの追加
      usedChunk(useg);
      //ゼロ初期化（本来はいらない）
      // bzero((void *)useg->begin,(seriesNum*ChunkSize));
      return (u32_t *)useg->begin;
    }
    pp_seg=&((*pp_seg)->np);
  }
  //確保できなかった
  panic("PFAallocator::cannot allocate segment");
}
/*特定の領域を空き領域リストから削除する関数
 * GRUBによってロードされたカーネルのコードおよびデータの領域が
 * 空き領域としてはじめ登録されてしまうので
 * それに対処するために必要な関数
 * ほかにもDMA領域の作成に利用可能である
 * 注意点　allocChunkが１度でも実行された後で行うべきではない（こんな関数の実装を複雑化したくないので）
 * 
 * */
u32_t * PFAllocator::removeChunk(u32_t bAddr_rm,u32_t eAddr_rm){
  Seg **pp_seg=&seg_free;
  //dirty......
  bAddr_rm=bAddr_rm&(~(ChunkSize-1));
  eAddr_rm=(eAddr_rm+(ChunkSize-1))&(~(ChunkSize-1));
  while((*pp_seg)!=NULL){
    //今見ている空き領域の先頭アドレスと末尾アドレス
    u32_t eAddr_chk=(*pp_seg)->begin+(*pp_seg)->blocks*ChunkSize;
    u32_t bAddr_chk=(*pp_seg)->begin;
    if(eAddr_rm >= bAddr_chk){
      u32_t chk_b,chk_e;
      chk_b=((bAddr_chk>=bAddr_rm) ? bAddr_chk:bAddr_rm);
      chk_e=((eAddr_chk<=eAddr_rm) ? eAddr_chk:eAddr_rm);
      u32_t rblk_begin,rblk_end,lblk_begin,lblk_end;
      lblk_begin=bAddr_chk;
      rblk_end=eAddr_chk;
      lblk_end=chk_b;
      rblk_begin=chk_e;
      if(lblk_end>lblk_begin){
	if(rblk_end>rblk_begin){
	  Seg *rSeg=getSeg();
	  (*pp_seg)->begin=lblk_begin;
	  (*pp_seg)->blocks=(lblk_end-lblk_begin)/ChunkSize;
	  rSeg->begin=rblk_begin;
	  rSeg->blocks=(rblk_end-rblk_begin)/ChunkSize;
	  rSeg->np=(*pp_seg)->np;
	  (*pp_seg)->np=rSeg;
	}
	else{
	  (*pp_seg)->begin=lblk_begin;
	  (*pp_seg)->blocks=(lblk_end-lblk_begin)/ChunkSize;
	}
      }else{
	if(rblk_end>rblk_begin){
	  (*pp_seg)->begin=rblk_begin;
	  (*pp_seg)->blocks=(rblk_end-rblk_begin)/ChunkSize;
	}
      }
    }
    pp_seg=&((*pp_seg)->np);
  } 
}
/*
 * 使用領域を返還し空き領域へ追加する関数
 */
void PFAllocator::freeChunk(u32_t *addr)
{
  Seg **pp_seg=&seg_used;
  while((*pp_seg)!=NULL){
    if((*pp_seg)->begin==(u32_t)addr){
      Seg *tmp=*pp_seg;
      *pp_seg=(*pp_seg)->np;
      addChunk(tmp);
      return;
    }
    pp_seg=&(*pp_seg)->np;
  }
  panic("PFAallocator::Ivalid Chunk freed");
}

/*
 * セグメント構造体の配列の要素を提供する関数（getSeg,freeSeg）
 * メモリ管理機構が使えないのでmallocを使う代わりに配列の要素を管理する
 * つまりSeg *seg=malloc()をSeg *seg=getSeg()とするための関数
 * */

PFAllocator::Seg *PFAllocator::getSeg()
{
  for(int i=0;i<MaxNumSeg;i++){
    if(segs[i].free){
      segs[i].free=FALSE;
      return &segs[i];
    }
  }
  panic("PFAllocator:cannot find free segment");
}
void PFAllocator::freeSeg(Seg *seg){
  seg->free=TRUE;
}





