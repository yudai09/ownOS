#!/bin/bash
#echo "copy hdd.img to /tmp/hdd.img"
echo "and mount hdd.img on /mnt/hdd"
#cp ./hdd.img /tmp/hdd.img
cp ./hdd.img /tmp/hdd.img
sudo mount -text2  -osync -oloop=/dev/loop1,offset=32256 /tmp/hdd.img /mnt/hdd
sudo chown yudai /mnt/hdd -R

