#!/bin/bash

#This is to find and set breakpoints within the VD.

readelf -s $1 | awk '($4 ~ "FUNC")&&((/'$2'/)&&((/_read/)||(/_write/)))&&(!/trace/) {print $8}' > func_names.txt
./ConstructBpFile func_names.txt bpFile.gdb

#cat bpFile.gdb

#invoke the virtual device
gnome-terminal -- /home/kyle/research/qemu-3.0.0-rc0/arm-softmmu/qemu-system-arm \
-M versatilepb -m 128M -kernel /home/kyle/research/VD-Debugging-Research/subject-drivers/qemu-bare-serials/test.bin \
-serial stdio -s -S

#invoke the gdb for the driver
gnome terminal -- "/home/kyle/research/gcc-arm-none-eabi-7-2018-q2-update/bin/arm-none-eabi-gdb;\
 target remote :1234; symbol-file /home/kyle/research/VD-Debugging-Research/subject-drivers/qemu-bare-serials/test.bin; continue"

#awk for PID - hope there is only one match
PID="$(ps -aux | awk '(/qemu-bare-serials/)&&(!/awk/) {print $2}')"

#sudo gdb -p <PID> and load the constructed script
sudo gdb -p "$PID" -x bpFile.gdb
