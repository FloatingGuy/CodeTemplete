#
# attach to the waiting agent-proxy
#
# usage:
# $ arm-eabi-gdb -q -x kgdb.gdb vmlinux
#

set remoteflow off
set remotebaud 115200
#set carrier-watch none
#set speed 115200
#target remote /dev/ttyUSB0
target remote localhost:4441
