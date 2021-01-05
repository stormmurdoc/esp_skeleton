#!/usr/bin/env sh
#
# Description: set up the OTA password
#
# Author: patrick@kirchhoffs.de
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
export ESP_OTA_PASSWORD="$(pass arduino/ota)"

