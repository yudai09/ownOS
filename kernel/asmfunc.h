#ifndef NASKFUNCH
#define NASKFUNCH
#include"Type.hpp"

void io_out8_asm(int port,int val);
void io_out16_asm(int port,int val);
u8_t io_in8_asm(int port);
u16_t io_in16_asm(int port);
void paging_on_asm(u32_t pagedir);
void load_cr3_asm(u32_t addr);
int notInKernel_asm();
void flashCache_asm();
void taskswitch_asm(int cs,int ss,int eip,int esp);
void ltr_asm(int sel);
void lidt_asm(unsigned int *addr);
void lgdt_asm(unsigned int *addr);
void interrupt0x31_asm(int millisec,int myid);
int kernel_call_asm(u32_t nr,u32_t arg1=0,u32_t arg2=0,u32_t arg3=0);//とくに引数に意味はない
void cli_asm(void);
void sti_asm(void);
u32_t get_esp(void);
/*ESPの値をprev->kernel_info->espに代入し
  同時にcurrent->kernel_info->espをESPに代入する*/
#define swap_esp(esp_prev,esp_next) \
  ({                                           \
    asm volatile ( "mov %%ebp,%0 \n\t" \
                   "mov %1,%%ebp \n\t" \
		   "mov %%esp,%%ebp \n\t"\
                   : "=b" (esp_prev): "c" (esp_next));\
    })

#define get_retaddr()		     \
  ({ u32_t __tmp;			     \
    asm volatile ( "mov 4(%%ebp),%0 \n\t"		\
		   : "=r" (__tmp): "r" (__tmp));	\
    __tmp; 						\
  })
#define get_ebp()		     \
  ({ u32_t __tmp;			     \
    asm volatile ( "mov %%ebp,%0 \n\t"		\
		   : "=r" (__tmp): "r" (__tmp));	\
    __tmp; 						\
  })
#define set_esp_ebp(esp) \
  ({ int __tmp;			     \
  asm volatile ( "mov %1,%%esp \n\t" \
		 "mov %1,%%ebp \n\t" \
		 : "=r" (__tmp): "a" (esp));	\
  })

#define jump(eip) \
  ({ \
  asm volatile ( "jmp %%eax \n\t" \
		 : "=r" (eip): "a" (eip));	\
  })


#endif
  

