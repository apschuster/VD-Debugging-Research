#!/bin/bash

#store the location of the ARM qemu
QEMU="/home/kyle/research/qemu-3.0.0-rc0/arm-softmmu/qemu-system-arm"

#find the source directory for the script and store it in DIR
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"

#This is to find and set breakpoints within the VD.

readelf -s $1 | awk '($4 ~ "FUNC")&&((/'$2'/)&&((/_read/)||(/_write/)))&&(!/trace/) {print $8}' > func_names.txt
$DIR/ConstructBpFile func_names.txt bpFile.gdb

#cat bpFile.gdb

#invoke the virtual device
gnome-terminal -- $QEMU \
-M versatilepb -m 128M -kernel ./test.bin \
-serial stdio -s -S

#invoke the gdb for the driver
gnome-terminal -- arm-none-eabi-gdb\
 -ex 'target remote localhost:1234' -ex 'symbol-file ./test.elf' -ex 'continue'

#awk for PID - exit on first match to avoid getting a false positive on the grep itself
PID="$(ps -aux | grep $QEMU | awk '{print $2; exit;}')"

#an echo for testing
#echo "$PID"

#sudo gdb -p <PID> and load the constructed script
sudo gdb -p "$PID" bpFile.gdb
