echo "unmount /mnt/hdd"
echo "Write back hdd.img?(y/N)"
read ans
if [ $ans = 'y' ];then
    echo "cp hdd.img"
    cp /tmp/hdd.img ./hdd.img
fi
sudo umount /mnt/hdd