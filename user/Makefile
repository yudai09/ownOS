#_/_/_/_/設定/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
.PHONY　:  all clean				#更新の有無に関わらず実行
.SUFFIXES : .elf .o .c .cpp .nas #サフィックスルールを有効にするための指示

CC=gcc
CPP=g++
NASM=nasm

CC_OPTIONS	=	$(INCLUDE) -O0 -nostdlib -nostartfiles -nodefaultlibs -m32 -g 
CPP_OPTIONS	=	$(INCLUDE) -O0 -fno-builtin -nostdlib -fno-exceptions -nostartfiles -m32 -g -fpermissive 
NASM_OPTIONS	=	-g
CC_EXTRA_OPTIONS= -Werror #警告でストップ
#INCLUDE =-I./include-libc
INCLUDE =-I./include
LIBRARY =-L ./lib
#LIBRARY =-L/usr/newlib-i386/lib
#INCLUDE		=
#_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

ELF_OBJS = user.elf
#LIBGRUE_OBJS := $(patsubst %.cpp,%.o,$(wildcard grue/*.cpp))
ALL_OBJS = $(ELF_OBJS) $(LIBGRUE_OBJS)

all: 
	make loadable_elf
loadable_elf: $(LIBGRUE_OBJS)
	make $(ELF_OBJS)
clean: 
	rm $(ALL_OBJS)
#サフィックスルール
.nas.o:
	$(NASM) $(NASM_OPTIONS) -f elf -o $@ $<
.c.o:
	$(CPP) -c -o $*.o $(CC_OPTIONS)  $<
.cpp.o:
	$(CPP) -c -o $*.o $(CPP_OPTIONS) $(CC_EXTRA_OPTIONS) $<
.cpp.elf: 
	$(CPP) -c -o $*.o $(CPP_OPTIONS) $(CC_EXTRA_OPTIONS) $*.cpp
	ld -melf_i386 -T executable.ld -o $*.elf $*.o $(LIBGRUE_OBJS)\
	lib/libc.a \
	lib/libm.a \
	lib/libnosys.a
#	lib/libpthread.a \
#	lib/libgcc.a 

#libc.a 
#動的文法チェック
check-syntax:
	$(CPP) $(CFLAGS) -pedantic -fsyntax-only $(CHK_SOURCES)
