#!/bin/sh
timeCheck=$(date | cut -c 12-19 | sed 's/://g');	#saves current time in a variable with format: hhmmss
echo "$timeCheck" > QR.dat				#timeCheck written in an empty/emptied QR.dat on line 1
echo "[$(date | cut -c 12-19)] Time check: $timeCheck, written on line 1 of QR.dat"

echo [$(date | cut -c 12-19)] Raspistill saving QR.jpg to tmpfs: /io/
raspistill -vf -hf -o /io/QR.jpg			#vertical and horizontal flip: (-vf, -hf) in output (-o) file (QR.jpg), saved on RAM, accessible at /io	
echo [$(date | cut -c 12-19)] Raspistill saved QR.jpg to tmpfs: /io/

zbarimg -q /io/QR.jpg >> /io/QR.dat			#print QR-code data (-q parameter suppresses other output) 
echo "[$(date | cut -c 12-19)] zbarimg QR poly length and code written on line 2 and 3 respectively of QR.dat"
cat /io/QR.dat						#print QR.dat to std output
