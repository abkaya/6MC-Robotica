#!/bin/bash
sudo mkdir /mnt/ramdisk/
sudo mount -t tmpfs -o size=100m tmpfs /mnt/ramdisk/
sudo chown pi /mnt/ramdisk/

##TO DO:
#add the following line to /etc/fstab in order for the ramdisk to exist upon reboot:
#tmpfs           /mnt/ramdisk/       tmpfs   nodev,nosuid,noexec,nodiratime,size=100m        0           0

##INFO:
#df -h output:
#Filesystem      Size  Used Avail Use% Mounted on
#/dev/sda1        20G  8.6G  9.8G  47% /
#udev            2.0G   12K  2.0G   1% /dev
#tmpfs           808M  904K  808M   1% /run
#none            5.0M     0  5.0M   0% /run/lock
#none            2.0G  172K  2.0G   1% /run/shm
#none            100M   16K  100M   1% /run/user
#tmpfs           100M     0  100M   0% /mnt/ramdisk

