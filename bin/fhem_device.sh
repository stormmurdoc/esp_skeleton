#!/usr/bin/env sh
#
# Description: build the FHEM device config
#
# Author: murdoc@storm-clan.de
#
# Configuration file: none
#
# Parameters: none
#

# -e: stops the script on errors
# -u: stops the script on unset variables
# -o pipefail : fail the whole pipeline if
#               a command fail

set -euo pipefail
clear
DEVICE=$(awk -F'"' '/DESP_HOSTNAME/ {print $2}' platformio.ini)
FHEMIODEV=$(awk -F'"' '/DFHEM_IODEV/ {print $2}' platformio.ini)

OUT="defmod $DEVICE MQTT2_DEVICE $DEVICE
attr $DEVICE IODev $FHEMIODEV
attr $DEVICE autocreate 1
attr $DEVICE devicetopic $DEVICE
attr $DEVICE event-on-change-reading .*
attr $DEVICE genericDeviceType switch
attr $DEVICE group Schalter
attr $DEVICE readingList \$DEVICETOPIC/rssi:.* rssi\\
\$DEVICETOPIC/cmnd:.* cmnd\\
\$DEVICETOPIC/output:.* output
attr $DEVICE setList on \$DEVICETOPIC/cmnd set_on\\
off \$DEVICETOPIC/cmnd set_off\\
toggle \$DEVICETOPIC/cmnd toggle\\
reboot \$DEVICETOPIC/cmnd reboot
attr $DEVICE stateFormat output
attr $DEVICE verbose 3
attr $DEVICE webCmd on:off

setstate $DEVICE off"
echo "$OUT" | xclip -selection clipboard
echo "$OUT"
