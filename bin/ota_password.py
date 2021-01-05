import subprocess
import os

# setup OTA password
out=subprocess.run(["pass", "arduino/ota", "/dev/null"], capture_output=True)
ota=out.stdout.decode('UTF-8').splitlines()
os.environ[ESP_OTA_PASSWORD] = ota[0]
