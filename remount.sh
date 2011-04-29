#!/bin/bash
echo "unmounting..."
sudo umount /mnt/hdd 
sleep 1
echo "mounting..."
sudo mount -text2  -osync -oloop=/dev/loop1,offset=32256 /tmp/hdd.img /mnt/hdd

