#!/usr/bin/python

import subprocess
import os

print("+++ Setting up OTA Password +++")

# setup OTA password
out=subprocess.run(["pass", "arduino/ota", "/dev/null"], capture_output=True)
os.environ["OTA_PASSWORD"]=out.stdout.decode('UTF-8').strip()

print("+++ Setting up Wifi credentials  +++")

# get WIFI credentials
out=subprocess.run(["pass", "wifi", "/dev/null"], capture_output=True)
my=out.stdout.decode('UTF-8').splitlines()

os.environ["STASSID"]=my[0]
os.environ["STAPSK"]=my[1]

