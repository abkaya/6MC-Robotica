#!/bin/bash
b=$(sed -n 2p QR.dat);
if ! echo $b | grep -q ldadin; then
echo "ok"
else
echo "nokbitch"
fi
