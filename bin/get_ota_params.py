import os
Import("env")
#print(env.Dump())
# already set
#env["UPLOAD_PROTOCOL"] = "espota"
# get OTA password.. from e.g. env variables, or from a text file, ..
pw = os.getenv("OTA_PASSWORD", default=None)
if pw is None:
    print("!! No ESP OTA password given!! Define the environment variable OTA_PASSWORD")
    # hard exit if wanted
    #env.Exit(1)
else:
    # append to upload flags
    env.Append(UPLOADERFLAGS = [
        "--auth",
        pw
    ])
# you can also set the "upload port" = ip / mDNS name here if needed
#env["UPLOAD_PORT"] = "192.168.1.192"
