#!/bin/sh
timeCheck=$(date | cut -c 12-19 | sed 's/://g');
echo "$timeCheck" > QR.dat
echo "[$(date | cut -c 12-19)] Time check: $timeCheck, written on line 1 of QR.dat"

echo [$(date | cut -c 12-19)] Raspistill saving QR.jpg to tmpfs: /io/
raspistill -o /io/QR.jpg
echo [$(date | cut -c 12-19)] Raspistill saved QR.jpg to tmpfs: /io/

#zbarimg -q parameter: only print read QR data
zbarimg -q /io/QR.jpg >> /io/QR.dat
echo "[$(date | cut -c 12-19)] zbarimg QR poly length and code written on line 2 and 3 respectively of QR.dat"
cat /io/QR.dat
