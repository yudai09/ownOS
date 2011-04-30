#_/_/_/_/設定/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
.PHONY　:  clean				#更新の有無に関わらず実行
.SUFFIXES : .o .c .cpp .nas #サフィックスルールを有効にするための指示

CC=gcc
CPP=g++
NASM=nasm

CC_OPTIONS	=	$(INCLUDE) -O0 -nostdlib -nostartfiles -nodefaultlibs -m32 -g 
CPP_OPTIONS	=	$(INCLUDE) -O0 -fno-builtin -nostdlib -fno-exceptions -nostartfiles -m32 -g -fpermissive
NASM_OPTIONS	=	-g
CC_EXTRA_OPTIONS= -Werror #警告でストップ
INCLUDE		= -I../include/os/ -I../include/

KERNEL_OBJS=loader.o kinit.o tk_printf.o mbi_read.o PFAllocator.o KGlobal.o kerror.o string.o\
	Descriptor.o Tss.o VarMem.o Timer.o Process.o irq.o asmfunc.o \
	isr.o init_hw.o \
	pinit.o  Ipc.o task.o timerHandler.o \
	syscall.o kmalloc.o \
	ata.o	\
	Device.o \
	Ext2.o \
	FileSystem.o\
	elf_loader.o\
	exec.o \
	vga.o \
	printf.o
kernel.bin:$(KERNEL_OBJS)
	ld -melf_i386 -T linker.ld -o kernel.bin $(KERNEL_OBJS)
clean:
	rm $(KERNEL_OBJS) kernel.bin
#サフィックスルール
.nas.o:
	$(NASM) $(NASM_OPTIONS) -f elf -o $@ $<
.c.o:
	$(CPP) -c -o $*.o $(CC_OPTIONS)  $<
.cpp.o:
	$(CPP) -c -o $*.o $(CPP_OPTIONS) $(CC_EXTRA_OPTIONS) $<
#動的文法チェック
check-syntax:
	$(CPP) $(CFLAGS) -pedantic -fsyntax-only $(CHK_SOURCES)
