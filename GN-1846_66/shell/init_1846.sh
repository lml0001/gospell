#!/bin/sh

echo "Loading Program"

cd /tmp/ko_gos/ && ./load3531a -i -osmem 192
route add default gw 120.120.120.1 dev eth0
/tmp/main
