#!/usr/bin/env sh
#
# $Id: template.sh,v 1.2 2020/10/19 17:36:15 murdoc Exp $
#
# Description
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
SSID="MURDOC1"
echo "-DWIFI_SID=\"$SSID\""
