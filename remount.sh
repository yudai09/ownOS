#!/bin/bash
echo "remounting..."
sudo umount /mnt/hdd || sudo mount -text2  -osync -oloop=/dev/loop1,offset=32256 /tmp/hdd.img /mnt/hdd

