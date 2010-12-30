#include"com.h"

void io_out8_asm(int port,int val){
  asm volatile(
	       "push %edx \n\t"
	       "mov 8(%ebp),%edx \n\t"
	       "mov 12(%ebp),%eax \n\t"
	       "out  %al,%dx \n\t"
	       "pop %edx \n\t"
	       );
}
void io_out16_asm(int port,int val){
  asm volatile(
	       "push %edx \n\t"
	       "mov 8(%ebp),%edx \n\t"
	       "mov 12(%ebp),%eax \n\t"
	       "out %ax,%dx \n\t"
	       "pop %edx \n\t"
	       );
}

u8_t io_in8_asm(int port){
  asm volatile(
	       "push %edx \n\t"
	       "mov 8(%ebp),%edx \n\t"
	       "mov 0,%eax \n\t"
	       "in  %dx,%al \n\t"
	       "pop %edx \n\t"
  );
  //EAXが返り値
}
u16_t io_in16_asm(int port){
  asm volatile(
	       "push %edx \n\t"
	       "mov 8(%ebp),%edx \n\t"
	       "mov 0,%eax \n\t"
	       "in  %dx,%ax \n\t"
	       "pop %edx \n\t"
  );

}

void paging_on_asm(u32_t pagedir){
  asm volatile(
	       "mov 8(%ebp),%eax \n\t"
	       "mov %eax,%cr3 \n\t"
	       "mov %cr0,%eax \n\t"
	       "or  $0x80000000,%eax \n\t"
	       "mov %eax,%cr0 \n\t"
	       );
}
void load_cr3_asm(u32_t addr){
	asm volatile(
			"mov 8(%ebp),%eax \n\t"
			"mov %eax,%cr3 \n\t"
	);

}
int notInKernel_asm(){
	asm volatile(
		"xor %eax,%eax \n\t"
		"mov %cs,%ax \n\t"
		"and $0x3,%ax \t\t"
	);
}

void flashCache_asm(){
  asm volatile(
	       "mov %cr3,%eax \n\t"
	       "mov %eax,%cr3 \n\t"
	       );
}
void taskswitch_asm(int cs,int ss,int eip,int esp){
	asm volatile(
			"pushl 12(%ebp)\n\t"
			"pushl 20(%ebp)\n\t"
			"pushl $0x1200 \n\t"
			"pushl 8(%ebp)\n\t "
			"pushl 16(%ebp)\n\t  "
			"mov 12(%ebp),%eax\n\t "
			"mov %ax,%ds\n\t "
			"mov %ax,%es\n\t "
			"mov %ax,%fs\n\t"
			"mov %ax,%gs\n\t"
//"hlt \n\t"
			"iretl \n\t"
	       );
}
void ltr_asm(int sel){
  asm volatile(
	       "ltr 8(%ebp) \n\t"
	       );
}
void lidt_asm(unsigned int *addr){
  asm volatile(
	       "mov 8(%ebp),%eax \n\t"
	       "lidt (%eax) \n\t"
	       );
}
void lgdt_asm(unsigned int *addr){
	asm volatile(
		       "mov 8(%ebp),%eax \n\t"
		       "lgdt (%eax) \n\t"
	);
}


void interrupt0x31_asm(int millisec,int myid){
	asm volatile(
	       "push %esi \n\t"
	       "push %edi \n\t"
	       "mov 8(%ebp),%esi\n\t "
	       "mov 12(%ebp),%edi\n\t "
	       "int $0x31\n\t "
	       "pop %edi\n\t "
	       "pop %esi\n\t " 
	       );
}

int kernel_call_asm(u32_t nr,u32_t arg1=0,u32_t arg2=0,u32_t arg3=0){
  asm volatile(
	       "push %eax \n\t"
	       "push %ebx \n\t"
	       "push %ecx \n\t"
	       "push %edx \n\t"
	       "mov 20(%ebp),%edx\n\t "
	       "mov 16(%ebp),%ecx\n\t "
	       "mov 12(%ebp),%ebx\n\t "
	       "mov 8(%ebp),%eax\n\t "
	       "int $0x30\n\t "
	       "pop %edx\n\t "
	       "pop %ecx\n\t "
	       "pop %ebx\n\t "
	       "addl $4,%esp \n\t"
	       //"pop %eax\n\t "
	       );

}
void cli_asm(void){
  asm volatile(
	       "cli \n\t"
	       );
}
void sti_asm(void){
  asm volatile(
	       "sti \n\t"
	       );
}

void get_esp(void){
  asm volatile(
	       "mov %esp,%eax"
	       );
}


//#define swap_esp(ADDR_KERNEL_INFO_TSS_ESP)                \

