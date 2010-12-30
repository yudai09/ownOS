#ifndef __KMALLOC_H__
#define __KMALLOC_H__

#include "kcom.h"

static const size_t KMALLOC_ALIGN = sizeof(void *);

struct kmalloc_header;

class kmalloc_header_ptr{
  const static int  KMALLOC_USED = 1;
  kmalloc_header *ptr;
  kmalloc_header *reset_ptr(kmalloc_header *ptr){
    return (kmalloc_header *)((u32_t)ptr & ~KMALLOC_USED);
  }
  kmalloc_header *reset_ptr(){
    return reset_ptr(this->ptr);
  }
public:
  kmalloc_header_ptr operator=(kmalloc_header *p){
    this->ptr = p;
    return *this;
  }
  kmalloc_header_ptr operator=(kmalloc_header_ptr *p){
    this->ptr = p->ptr;
    return *this;
  }
  bool operator==(kmalloc_header_ptr *p){ return p->pointer()==this->pointer(); }
  
  kmalloc_header *operator->(){ return reset_ptr(ptr); }
  
  kmalloc_header *operator*(){ return reset_ptr(ptr); }

  operator u32_t(){    return (u32_t)reset_ptr();  }
  void set_used(bool used){
    if(used)
      ptr = (kmalloc_header *)((u32_t)ptr | KMALLOC_USED);
    else
      ptr = (kmalloc_header *)((u32_t)ptr & ~KMALLOC_USED);
  }
  bool is_used(){
    return (u32_t)ptr & KMALLOC_USED;
  }
  kmalloc_header *pointer(){
    return ptr;
  }
  
  //  kmalloc_header *operator *();
};
struct kmalloc_header {
  kmalloc_header_ptr next;
  u32_t size;
};

void *kmalloc(size_t size);
void kfree(void *addr);
void *operator new(size_t size);
void *operator new[](size_t size);
void operator delete(void *p);
void operator delete[](void *p);


#endif
