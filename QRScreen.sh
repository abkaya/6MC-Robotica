#!/bin/sh
# READ: This script _can_ be used to temporarily start a screen, which will execute the decoder script
# as a background process, however, this isn't not necessary, so we're leaving it out.
# it performs the check on whether or not a certain screen is already existent or not as to not recreate multiple screens unnecessarily.
function UseScreen {
        echo "[$(date | cut -c 12-19)] QRDecoder Screen is still idle, using that one."
        #stuff the active screen with the script start command
        screen -r QRDecoder -X stuff "sh QRDecoder.sh"
        echo "[$(date | cut -c 12-19)] QRDecoder.sh ran in active screen QRDecoder. Use screen -x QRDecoder to view screen."
}

function NewScreen {
        echo "[$(date | cut -c 12-19)] No idle QRDecoder screen detected. Starting now."
        screen -dm -S QRDecoder ./QRDecoder.sh
        echo "[$(date | cut -c 12-19)] New QRDecoder screen started and ran QRDecoder.sh script in it."
}

grepOut=$(screen -ls);
if echo "$grepOut" | grep -q QRDecoder; then
        UseScreen
else
        NewScreen
fi

