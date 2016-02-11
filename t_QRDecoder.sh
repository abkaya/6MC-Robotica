#!/bin/bash
#Test script in ubuntu; mind the output locations and expression after the shebang (line 1)
timeCheck=$(date | cut -c 12-19 | sed 's/://g');
echo "$timeCheck" > QR.dat
echo "[$(date | cut -c 12-19)] Time check: $timeCheck, written on line 1 of QR.dat"

echo "[$(date | cut -c 12-19)] Raspistill saving QR.jpg to tmpfs: /io/"
#removed line. QR.jpg exists for this test
echo "[$(date | cut -c 12-19)] Raspistill saved QR.jpg to tmpfs: /io/"

#zbarimg -v parameter: verbose,sed -n parameter: line number of verbose output, -i: append to next line
zbarimg -q QR.jpg >> QR.dat
echo "[$(date | cut -c 12-19)] zbarimg QR poly length and code written on line 2 and 3 respectively of QR.dat"
cat QR.dat

