#!/bin/bash
#lubuntu test version
GREEN='\033[0;32m'
NC='\033[0m'
ORANGE='\033[0;33m'
CYAN='\033[0;36m'
BG='\033[44;1;37m'
RED='\033[0;31m'

#ifdef variant; use in code: $debug&& { code here }
#do not comment debug out. Replace true by false if necessary
debug=true
error=false
errorTest=false
rm -rf /mnt/ramdisk/*

function ErrorCheck {
	grepOut="$(tail /mnt/ramdisk/raspistill.dat)";
	grep0Out=$(sed -n 1p /mnt/ramdisk/QR.dat);
	grep2Out=$(sed -n 2p /mnt/ramdisk/QR.dat);

	#test grepOut for Camera Error
	$errorTest&& grepOut="mmal"

	if echo "$grepOut" | grep -q mmal;then
		error=true
		sed -i '3i\1' /mnt/ramdisk/QR.dat                
		$debug&& echo -e "${BG}[$(date | cut -c 12-19)]${NC} ERROR CODE 1: Camera Error : [${RED}NOK${NC}]"
	elif ! echo "$grep2Out" | grep -q lading;then
		error=true
		sed -i '3i\2' /mnt/ramdisk/QR.dat                
		$debug&& echo -e "${BG}[$(date | cut -c 12-19)]${NC} ERROR CODE 2: No QR data Error : [${RED}NOK${NC}]"
	elif [[ "${#grep0Out}" -lt "2" ]];then
		error=true
		sed -i '3i\3' /mnt/ramdisk/QR.dat                
		$debug&& echo -e "${BG}[$(date | cut -c 12-19)]${NC} ERROR CODE 3: Other Error : [${RED}NOK${NC}]"
	else
		sed -i '3i\0' /mnt/ramdisk/QR.dat
	fi
	printOk
}

function printOk {
	if [[ "$error" == "false" ]];then
		$debug&& echo -e "${BG}[$(date | cut -c 12-19)]${NC} Raspistill saved ${ORANGE}QR.jpg${NC} to ${CYAN}tmpfs: /mnt/ramdisk/ (RAM)${NC} : [${GREEN}OK${NC}]"
		$debug&& echo -e "${BG}[$(date | cut -c 12-19)]${NC} QR-code data written on line 2  of ${ORANGE}/mnt/ramdisk/QR.dat${NC} : [${GREEN}OK${NC}]"
		$debug&& echo -e "${BG}[$(date | cut -c 12-19)]${NC} QR-code data read. Time check: ${ORANGE}$timeCheck${NC}, written on line 1 of ${ORANGE}/mnt/ramdisk/QR.dat${NC} : [${GREEN}OK${NC}]"
	fi
}



#saves current time in a variable with format: hhmmss, to be compared by the compiled c program
timeCheck=$(date | cut -c 12-19 | sed 's/://g');
#the compiled c program will know the QR code info is not yet available when line 1 equals to 0
echo "0" > /mnt/ramdisk/QR.dat



$debug&& echo -e "${BG}[$(date | cut -c 12-19)]${NC} Raspistill saving ${ORANGE}QR.jpg${NC} to ${CYAN}tmpfs: /mnt/ramdisk/ (RAM)${NC}"
#Raspistill: vertical and horizontal flip: (-vf, -hf) in output (-o) file (QR.jpg), saved on RAM, accessible at /io 
#-w -h : width and height; lowest 640x480; should hopefully be faster at lower quality shots. -q 10: quality (0-100). 100: uncompressed. 10 should deliver lower quality.
raspistill -o /mnt/ramdisk/QR.jpg >> /mnt/ramdisk/raspistill.dat
wait

#print QR-code data (-q parameter suppresses other output) 
zbarimg -q /mnt/ramdisk/QR.jpg > /mnt/ramdisk/QRTemp
QRData=$(cat /mnt/ramdisk/QRTemp)
echo $QRData >> /mnt/ramdisk/QR.dat

#add an additional space to QR.dat in order to make sure we have at least 3 lines in QR.dat (redundancy for follow-up sed commands)
echo $'\n' >> /mnt/ramdisk/QR.dat


#sed -i "2i\$QRData" /mnt/ramdisk/QR.dat
#sed -i '2d' /mnt/ramdisk/QR.dat
sed -i 's/QR-Code://g' /mnt/ramdisk/QR.dat
#QR-code data is available so we can now replace line 1 with the actual timeCheck value
sed -i '1s/.*/'$timeCheck'/' /mnt/ramdisk/QR.dat

ErrorCheck

$debug&& echo -e "${CYAN}--------------------------------${NC}"
#print QR.dat to std output.
$debug&& head -n 3 /mnt/ramdisk/QR.dat
$debug&& echo -e "${CYAN}--------------------------------${NC}"

