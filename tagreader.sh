#!/bin/bash

#=======================================================
#
#  tagreader.sh |  
#  Written by Abdil Kaya & Robin Janssens
#
#=======================================================

echo scriptor APDU | grep "<" | tr -d ' ' | sed 's\<\\g' | sed 's\:Normalprocessing\\g'
