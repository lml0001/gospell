#!/bin/sh

echo "System Loading"

ifconfig eth0 120.120.120.10 netmask 255.255.255.0

echo "Decompressing Program"

cd /tmp
tar -xzf ./software.tar.gz

/tmp/init.sh
