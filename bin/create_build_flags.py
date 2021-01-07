import subprocess
import os

# setup OTA password
out=subprocess.run(["pass", "arduino/ota", "/dev/null"], capture_output=True)
ota=out.stdout.decode('UTF-8').splitlines()
os.environ["OTA_PASSWORD"]=ota[0]

## get WIFI credentials
out=subprocess.run(["pass", "wifi", "/dev/null"], capture_output=True)
wifi=out.stdout.decode('UTF-8').splitlines()

## get broker infos
out=subprocess.run(["pass", "broker", "/dev/null"], capture_output=True)
broker=out.stdout.decode('UTF-8').splitlines()

## get git revision
revision = (
    subprocess.check_output(["git", "rev-parse", "HEAD"])
    .strip()
    .decode("utf-8")
)

## create build flags (macros)
print("-DGIT_REV='\"%s\"'" % revision)
print("-DOTA_PASSWORD='\"%s\"'" % ota[0])
print("-DWIFI_SSID='\"%s\"'" % wifi[1])
print("-DWIFI_PASSWORD='\"%s\"'" % wifi[0])
print("-DMQTT_PASSWORD='\"%s\"'" % broker[0])
print("-DMQTT_SERVER='\"%s\"'" % broker[1])
print("-DMQTT_PORT=%s" % broker[2])
