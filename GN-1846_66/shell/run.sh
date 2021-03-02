#!/bin/sh

echo "System Loading"

ifconfig eth0 120.120.120.10 netmask 255.255.255.0

echo "Decompressing Program"

cd /tmp

cp /mnt/mtd/software.tar.gz ./
tar -xzf ./software.tar.gz
rm -f ./software.tar.gz
#cp -rf /mnt/mtd/program/* /tmp

/tmp/init.sh
