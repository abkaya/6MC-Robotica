#!/bin/bash
#========================================
#
#  Script to remove remaining scripts
#  Written by Abdil Kaya & Robin Janssens
#
#========================================

if [[ $USER == "pi" ]]; then
	rm -f QRDecoder.sh
	rm -f tagreader.sh
	rm -f mkRamdisk.sh
	rm -f cleanup.sh
else
	echo "ERROR: Cleanup is not meant for this user"
fi
