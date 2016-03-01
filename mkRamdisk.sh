#!/bin/bash

#=======================================================
#
#  QRDecoder.sh |  
#  Written by Abdil Kaya & Robin Janssens
#
#=======================================================
GREEN='\033[0;32m'
NC='\033[0m'
ORANGE='\033[0;33m'
CYAN='\033[0;36m'
BG='\033[44;1;37m'
RED='\033[0;31m'



CreateDisk() {
	sudo mkdir /mnt/ramdisk/
	sudo mount -t tmpfs -o size=100m tmpfs /mnt/ramdisk/
	sudo chown pi /mnt/ramdisk/
	CheckDisk
}

CheckDisk() {
	dirMade=$(ls -laG / | grep /mnt/ramdisk)
	diskMade=$(df -h | grep /mnt/ramdisk)
	if [[ ! $dirMade && ! $diskMade ]]; then
		CreateDisk
		echo -e "${BG}[$(date | cut -c 12-19)]${NC} RAMDISK /mnt/ramdisk/ not found. Creating now : [${ORANGE}INFO${NC}]"
	else
		echo -e "${BG}[$(date | cut -c 12-19)]${NC} RAMDISK /mnt/ramdisk found or created : [${GREEN}INFO${NC}]"
	fi
}

CheckDisk
#to unallocate this space; mke2fs -m 0 /mnt/ramdisk

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

