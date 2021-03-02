#!/bin/sh

echo "Loading Program"

insmod ./fpga.ko
insmod ./fan.ko
insmod ./fpga_config_driver.ko
insmod ./tuner_device.ko

mknod /dev/fpga c 211 1 
mknod /dev/fan  c 210 1
mknod /dev/fpgaconfig c 231 0
mknod /dev/tuner_device c 233 1

/tmp/main