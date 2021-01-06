#SHELL := /bin/bash
#PATH := /usr/local/bin:$(PATH)


all:
	platformio -f -c vim run

upload:
	source ./bin/set_my_vars.sh;platformio -f -c vim run --target upload --target monitor

clean:
	platformio -f -c vim run --target clean

program:
	platformio -f -c vim run --target program

uploadfs:
	platformio -f -c vim run --target uploadfs

update:
	platformio -f -c vim update

erase:
	platformio -f -c vim run --target erase

dump:
	platformio -f -c vim run --target envdump
