#!/usr/bin/env sh
#
# Description: set up the OTA password
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

OUT="$(pass arduino/ota)"

if [ $? -eq 0 ];then
    if [ ! -z "$OUT" ];then
        export ESP_OTA_PASSWORD="$OUT"
    else
        echo "--- Error: could not retriew the password from pass! ---"
        exit 1
    fi
else
    echo "--- Error launching pass - maybe not installed? ---"
    exit 2
fi
