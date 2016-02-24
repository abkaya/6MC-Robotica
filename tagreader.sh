#!/bin/bash

echo scriptor APDU | grep "<" | tr -d ' ' | sed 's\<\\g' | sed 's\:Normalprocessing\\g'
