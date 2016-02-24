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

function ErrorCheck {
	grepOut="$(tail QR.dat)";
	grep0Out=$(sed -n 1p QR.dat);
	grep2Out=$(sed -n 2p QR.dat);

	#test grepOut for Camera Error
	$errorTest&& grepOut="mmal"

	if echo "$grepOut" | grep -q mmal;then
		error=true
		echo "1" >> QR.dat
		$debug&& echo -e "${BG}[$(date | cut -c 12-19)]${NC} ERROR CODE 1: Camera Error : [${RED}NOK${NC}]"
	elif ! echo "$grep2Out" | grep -q lading;then
		error=true
		echo "2" >> QR.dat
		$debug&& echo -e "${BG}[$(date | cut -c 12-19)]${NC} ERROR CODE 2: No QR data Error : [${RED}NOK${NC}]"
	elif [[ "${#grep0Out}" -lt "2" ]];then
		error=true
		echo "3" >> QR.dat
		$debug&& echo -e "${BG}[$(date | cut -c 12-19)]${NC} ERROR CODE 3: Other Error : [${RED}NOK${NC}]"
	fi
	printOk
}

function printOk {
	if [[ "$error" == "false" ]];then
		$debug&& echo -e "${BG}[$(date | cut -c 12-19)]${NC} Raspistill saved ${ORANGE}QR.jpg${NC} to ${CYAN}tmpfs: /io/ (RAM)${NC} : [${GREEN}OK${NC}]"
		$debug&& echo -e "${BG}[$(date | cut -c 12-19)]${NC} QR-code data written on line 2  of ${ORANGE}/io/QR.dat${NC} : [${GREEN}OK${NC}]"
		$debug&& echo -e "${BG}[$(date | cut -c 12-19)]${NC} QR-code data read. Time check: ${ORANGE}$timeCheck${NC}, written on line 1 of ${ORANGE}/io/QR.dat${NC} : [${GREEN}OK${NC}]"
	fi
}




timeCheck=$(date | cut -c 12-19 | sed 's/://g');        #saves current time in a variable with format: hhmmss, to be compared by the compiled c program
echo "0" > QR.dat                                   	#the compiled c program will know the QR code info is not yet available when line 1 equals to 0
#line three starts with error code 0: nothing is wrong
sed -i '3s/.*/0/' QR.dat
$debug&& echo -e "${BG}[$(date | cut -c 12-19)]${NC} Raspistill saving ${ORANGE}QR.jpg${NC} to ${CYAN}tmpfs: /io/ (RAM)${NC}"

zbarimg -q QR.jpg >> QR.dat #print QR-code data (-q parameter suppresses other output) 
sed -i '2d' QR.dat
sed -i 's/QR-Code://g' QR.dat
sed -i '1s/.*/'$timeCheck'/' QR.dat			#QR-code data is available so we can now replace line 1 with the actual timeCheck value

ErrorCheck

$debug&& echo -e "${CYAN}--------------------------------${NC}"
$debug&& cat QR.dat                                          #print QR.dat to std output.i
$debug&& echo -e "${CYAN}--------------------------------${NC}"
