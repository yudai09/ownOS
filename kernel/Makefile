#_/_/_/_/����/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
.PHONY��:  clean				#������̵ͭ�˴ؤ�餺�¹�
.SUFFIXES : .o .c .cpp .nas #���ե��å����롼���ͭ���ˤ��뤿��λؼ�

CC=gcc
CPP=g++
NASM=nasm

CC_OPTIONS	=	$(INCLUDE) -O0 -nostdlib -nostartfiles -nodefaultlibs -m32 -g 
CPP_OPTIONS	=	$(INCLUDE) -O0 -fno-builtin -nostdlib -fno-exceptions -nostartfiles -m32 -g
NASM_OPTIONS	=	-g
CC_EXTRA_OPTIONS=-Wall #-Wextra -Werror #�ٹ�ǥ��ȥå�
#INCLUDE	=	-I./include
INCLUDE		=
#_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
OBJS=loader.o kinit.o tk_printf.o mbi_read.o PFAllocator.o KGlobal.o kerror.o string.o\
	Descriptor.o Tss.o VarMem.o Timer.o Process.o irq.o asmfunc.o \
	init_pit.o init_pic.o  isr.o init_KBC.o \
	taskInit.o taskSys.o taskPman.o taskIdle.o Ipc.o task.o timerHandler.o \
	syscall.o kmalloc.o \
	ata.o	\
	Device.o \
	Ext2.o
#	IDEDriver.o

kernel.bin:$(OBJS)
	ld -melf_i386 -T linker.ld -o kernel.bin $(OBJS)

clean:
	rm $(OBJS) kernel.bin

#���ե��å����롼��
.nas.o:
	$(NASM) $(NASM_OPTIONS) -f elf -o $@ $<
.c.o:
	$(CPP) -c -o $*.o $(CC_OPTIONS)  $<
.cpp.o:
	$(CPP) -c -o $*.o $(CPP_OPTIONS) $(CC_EXTRA_OPTIONS) $<

#ưŪʸˡ�����å�
check-syntax:
	$(CPP) $(CFLAGS) -pedantic -fsyntax-only $(CHK_SOURCES)