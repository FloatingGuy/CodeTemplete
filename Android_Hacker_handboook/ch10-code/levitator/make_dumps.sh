#!/bin/sh

mkdir dumps 2> /dev/null

for ii in `seq 1 100`; do 
  echo -n "waiting for device...";
  adb wait-for-device shell /data/local/tmp/lev-dump /data/local/tmp/dump
  adb pull /data/local/tmp/dump dumps/dump.$ii
  adb shell rm /data/local/tmp/dump 
  adb reboot
done

