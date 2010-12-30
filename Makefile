.PHONY: KERNEL clean
KERNEL:
	make -C kernel
	cp kernel/kernel.bin /mnt/hdd
clean:
	make -C kernel clean
#覚書
#マウントオプション「-o sync」はイメージファイル(/tmp/hdd.img)と
#マウントしたファイル（/mnt/hdd）が同期するために付けた
#　
## DO NOT DELETE

kernel/kinit.o: kernel/tk_printf.h
