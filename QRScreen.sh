raspistill -o /io/QR.jpg
zbarimg -v /io/QR.jpg | sed -n '1 p' >> QR.dat
