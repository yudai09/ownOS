#!/bin/bash

qemu -m 128 -localtime -d int -hda /tmp/hdd.img -vga std -s -S &
sleep 1
gdb ./user/user.elf
#break main
#gdb ./kernel/kernel.bin
#break FileSystem.cpp:115
continue

