#!/bin/sh

echo "Loading Program"

cd /tmp/ko_gos/ && ./load3519v101 -i -sensor0 ov4689 -sensor1 ov4689 -osmem 192 -offline
route add default gw 120.120.120.1 dev eth0
/tmp/main
