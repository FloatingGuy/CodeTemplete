#!/bin/sh
while true; do
  sleep 4
  adb shell 'su -c /data/local/tmp/attach.sh' >> adb.log 2>&1
done
