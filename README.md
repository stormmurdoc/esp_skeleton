# ESP Skeleton

Skeleton project for new ESP8266 projects.

## Arduino Example Code

This code will only blink the internal blue LED of a ESP8266 (wemos d1 or similiar).

## Requirements

This project use pass for storing user credentials.
Take a look in bin/create_build_flags.py for further details.

The ESP_OTA_PASSWORD var is exported via bin/set_my_vars.sh

Please make sure that pass is installed & configured.

## First run

To be able to use the OTA update, the following two entries
in the platformio.ini must be changed.

    ; Over the air settings
    upload_protocol = espota
    upload_port = 192.168.1.108

Additionally the entry "" must be commented out:

    ; initial setup via
    upload_protocol = esptool

## How to build this project?

    make

## How to upload this project?

    make upload_protocol

## How to debug the environment vars?

    make dump

## How to clean up the cache?

    make clean
