#!/bin/bash

qemu -m 128 -localtime -d int -hda /tmp/hdd.img -s -S &
sleep 1
gdb ./kernel/kernel.bin

continue
