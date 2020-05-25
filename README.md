# Research

VD-Debugging-Research contains code and documentation pertaining to academic research into debugging virtual devices/drivers.
* Refer to this fork for all of the most recent files. Darins github is the old directory that existed before I (Alex Schuster) started working on this. Refer to that for older versions of code to see what was changed. This directory is called `VD-Debugging-ResearchNew` within the OCI VM.

## pythonParser
* This directory contains the python script that is utitilized in conjuction with the .gdb file that is created from the SetFuncBps. There is a Readme.md that goes into more details about this script within said directory.

## subject-drivers
* This directory contains all of the subject drivers that are tested with the QEMU virtual emulator. `qemu-bare-serials` is the one that has been tested the most (it is also the simplest one). Look into Darin's/Kyle's research report for information regarding the specifics of these drivers. It also contains a Readme.md that goes into more detail within said directory.

## SetFuncBps
* This directory contains the SetFuncBps. It also contains the .cpp file that works with the SetFuncBps to find all of the Read/Write functions with the ARM instance of QEMU.
* This script searches for all Read/Write functions within ARM, takes those names in the .cpp file, and generates a .gdb file that sets breakpoints and extracts information from all of the relevant breakpoints. (The .gdb file is sourced within the script when properly hooked into the gdb session.)
