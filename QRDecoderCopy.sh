#!/bin/shi
echo [$(date | cut -c 12-19)] Rapistill saving QR.jpg to tmpfs: /io/
raspistill -o /io/QR.jpg
echo [$(date | cut -c 12-19)] Rapistill saved QR.jpg!
zbarimg -v /io/QR.jpg | sed -n '1 p' >> QR.dat
timeCheck=$(date | cut -c 12-19 | sed s/://g);
echo "[$(date | cut -c 12-19)] Time safety :$timeCheck check written on line 1 of QR.dat"
echo "[$(date | cut -c 12-19)] zbarimg QR output written on line 2 of QR.dat"
