#include "kcom.h"
#include "kmalloc.h"

//centinels
kmalloc_header_ptr base;
kmalloc_header_ptr next_free;

void init_kmalloc(){
  u32_t *aligned = pFAllocator.allocChunk(0x20);
  base = next_free = (kmalloc_header *)aligned;

  base->next = base;//循環
  base->size = next_free->size = 0x1000*0x20;
  kprintf("aligned %x headersize %d aligned by %d \n",aligned,sizeof(kmalloc_header),KMALLOC_ALIGN);
}

void *kmalloc(size_t size){
  kmalloc_header_ptr p = next_free;
  //アライン
  u32_t rsize = (size+sizeof(kmalloc_header)+KMALLOC_ALIGN -1)&(-KMALLOC_ALIGN);

  do{
    if(rsize <= p->size){
      next_free = p;
      p = (kmalloc_header *)((u32_t)p+next_free->size-rsize);
      p.set_used(true);
      p->next = next_free->next;
      p->size = rsize;
      next_free->next = p;
      next_free->size -= rsize;
      //size==0になったら?
      //未定
      return (u32_t *)((u32_t)p+sizeof(kmalloc_header));
    }
    //以下のwhileは無限ループにならない.
    //base->next=base (!=base|KMALLOC_USED)なので
    do{
      p = p->next;
    }while(p.is_used());
  }while(p!=base);
  panic("malloc error \n");
  return NULL;
}


void kfree(void *addr){
  //  kprintf("kfree \n");
  kmalloc_header_ptr freed;
  kmalloc_header_ptr prev,next;
  bool is_next_used,is_prev_used;

  freed = (kmalloc_header *)((u32_t)addr-sizeof(kmalloc_header));

  {
    kmalloc_header_ptr p=base;
    kmalloc_header_ptr tp;

    while((tp = p->next) != freed){
      p =tp;
    }

    prev = p;
    next = tp->next;
    is_prev_used = p.is_used();
    is_next_used = next.is_used();
  }

  //端処理
  if(freed == base){
    is_prev_used=true;
  }
  if(next == base){
    is_next_used=true;
  }
  if(!is_prev_used){//concat to big area
    prev->size += freed->size;
    prev->next = freed->next;
    if(!is_next_used){
      prev->size += next->size;
      prev->next = next->next;
    }
  }else if(!is_next_used){//concat to big area
    prev->next.set_used(false);
    freed->size += next->size;
    freed->next = next->next;
  }else{//cannot concat
    freed.set_used(false);
    prev->next = freed;
  }
}

void *operator new(size_t size)
{
  return kmalloc(size);
}
 
void *operator new[](size_t size)
{
  //  kprintf("new[] size %d \n",size);
  return kmalloc(size);
}
 
void operator delete(void *p)
{
  kfree(p);
}
 
void operator delete[](void *p)
{
  kfree(p);
}
