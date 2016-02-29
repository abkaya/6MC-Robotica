#!/bin/bash

#=======================================================
#
#  tagreader.sh
#  Written by Abdil Kaya & Robin Janssens
#
#=======================================================

scriptor APDU 2>/dev/null | grep '<' | tr -d ' ' | sed 's\<\\g' | sed 's\:Normalprocessing.\\g'
