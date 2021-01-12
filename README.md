# ESP Skeleton

## Scope

This skeletion (template) aims to be a framework for ESP8266 Arduino projects.

## Range of functions

* Wifi connection
* NTP time synchronization
* MQTT connection
* Internal LED flashs when MQTT messages are send/received
* all configs are in the platform.io file
* uses pass as password storage and other configurations
* can create FHEM device configuration (MQTT device)
* makefile integration
* Over the air update & monitoring (OTA)

## Arduino Example Code

The sample code included is used to switch an LED on and off via FHEM using MQTT.

## Requirements

### Pass

This project use pass for storing user credentials.
Take a look in bin/create_build_flags.py for further details.

The ESP_OTA_PASSWORD var is exported via bin/set_my_vars.sh

Please make sure that pass is installed & configured.

### Platform IO

Please install platformio with you os packet manager.

## How to build this project?

    make

## How to upload this project via serial connection?

    make serial

## How to upload via OTA?

    make ota

## How to debug the environment vars?

    make dump

## How to clean up the cache?

    make clean

## How to edit code via vim?

    make edit

## How to clean build environment?

    make clean

## How can I create a FHEM device configuration?

    make fhem

Note: The device config will be copied into the system clipboard automatically.

## How can I monitor the device?

    make monitor
