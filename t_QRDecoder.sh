#!/bin/bash
#lubuntu test version
GREEN='\033[0;32m'
NC='\033[0m'
ORANGE='\033[0;33m'
CYAN='\033[0;36m'
BG='\033[44;1;37m'

timeCheck=$(date | cut -c 12-19 | sed 's/://g');        #saves current time in a variable with format: hhmmss, to be compared by the compiled c program
echo "0" > QR.dat                                   	#the compiled c program will know the QR code info is not yet available when line 1 equals to 0

echo -e "${BG}[$(date | cut -c 12-19)]${NC} Raspistill saving ${ORANGE}QR.jpg${NC} to ${CYAN}tmpfs: /io/ (RAM)${NC} : [${GREEN}OK${NC}]"
echo -e "${BG}[$(date | cut -c 12-19)]${NC} Raspistill saved ${ORANGE}QR.jpg${NC} to ${CYAN}tmpfs: /io/ (RAM)${NC} : [${GREEN}OK${NC}]"

zbarimg -q QR.jpg >> QR.dat				#print QR-code data (-q parameter suppresses other output) 
sed -i '2d' QR.dat
sed -i 's/QR-Code://g' QR.dat
echo -e "${BG}[$(date | cut -c 12-19)]${NC} QR-code data written on line 2  of ${ORANGE}/io/QR.dat${NC} : [${GREEN}OK${NC}]"
sed -i '1s/.*/'$timeCheck'/' QR.dat			#QR-code data is available so we can now replace line 1 with the actual timeCheck value
echo -e "${BG}[$(date | cut -c 12-19)]${NC} QR-code data read. Time check: ${ORANGE}$timeCheck${NC}, written on line 1 of ${ORANGE}/io/QR.dat${NC} : [${GREEN}OK${NC}]"

echo -e "${CYAN}--------------------------------${NC}"
cat QR.dat                                          #print QR.dat to std output.i
echo -e "${CYAN}--------------------------------${NC}"
