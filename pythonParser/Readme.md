# Breakpoint Parsing Script

This program, `breakPoint_parser.py`, will work in unison with the SetFunc breakpoint script. The way this works is after all the breakpoints have been set, the .gdb script that is sourced within the SetFuncBPS will also have a command to execute this program in the shell. This program takes the logging file as an input (which is automatically appended to while gdb is running), and retrieves the local variables/arguments from the most recent breakpoint that has been stopped on. From there, the script generates a new .gdb file that sets all of those variables to a new value using the "set" command. This file will be sourced after this python script is ran within the SetFuncBPS. This will occur for every breakpoint that is stopped at

If you want to see the code that references this script as an input, check out SetFuncBps/WriteBps.cpp line 54.

* Please check the `Bugs/Limitations` header. There is important notes that must be mentioned regarding this program's current limitations that future developers need to be aware of.

## Getting Started

These instructions will go over the nuances of the program such as whats necessary to set it up, required packages, inputs/outputs, and how to use it.

### Prerequisites

Required packages/software

```
Linux/Unix environment is required to run this properly
python 3.0 or newer (3.6 in my case)
SetFuncBPS setup properly (Which it should be with the VM OCI reference download)
```

### Installing

This setup simply describes how to install python 3.x on your system. If you already have python 3, dont worry about this. If you're using the VM OCI reference, this is unecessary.

Check python version

```
python -V
or
python3 -V
```

If not python version 3 (or native python doesn't point to python 3) continue.

Install python 3.6 (if python3 is not on machine) using the following commands.

```
wget https://www.python.org/ftp/python/3.6.3/Python-3.6.3.tar.xz
tar xJf Python-3.6.3.tar.xz
cd Python-3.6.3
./configure
make
make install
```

Afterwards, change native python to point to python3 by editing .bashrc file. Go to home directory and edit .bashrc with following command.

```
cd ~/
vim .bashrc
```

Add the following line at any point in the file (make sure that a similar line does not already exist, if it does, modify to the one shown below)

```
alias python='/usr/bin/python3.6'
```

### Inputs/Outputs 

* Inputs: `gdb.txt` (The logging file that is automatically generated from a gdb session)
  - `gdb.txt`: This is .txt file that is generated from the gdb session after the SetFuncBPS is ran. It is the logging file, which means it copies all of the output from the current gdb session. This program automatically knows to reference this, assuming you're running the program in the correct directory that is (same directory as the driver you're wishing to use the SetFuncBPS on).
  
* Outputs: `changeVars.gdb`
  - `changeVars.gdb`: This is what the program outputs. It is a .gdb file that contains commands to change the variables for the most recent breakpoint that the SetFuncBPS has stopped on. This file will be used with the SetFuncBPS to be sourced within gdb to change these variables (yes, you're able to source a .gdb script that can in turn source another .gdb script, it will perform in that expected order.). If you want to see the code that references this script's output, check out SetFuncBps/WriteBps.cpp line 55.

### How to Run Program

* Running this program is pretty simple. I designed it such that it works with the pre-existing SetFuncBps by modifying the script slightly to reference this python program and the output it produces within the bpFile.gdb that is sourced from the SetFuncBps.

* That being said, in order to run this program all one has to do is make sure that the `breakPoint_parser.py` program is within the driver directory that they want (subject-drivers/qemu-bare-serials already has this setup, use as reference), and then call the SetFuncBps by typing "SetFuncBps" within that directory. This still operates off the normal functionality of the SetFuncBps, so the .elf and .bin files must be there- in other words, the driver will have to have it`breakPoint_parser.py`s executable made using the "makefile".

## Bugs/Limitations

At the time of creating this program, I have been unable to get the SetFuncBps working in its intended fashion. By that, I mean the bash script seems to halt when the qemu process ID is hooked into the gdb session. I'm unsure why this is happening, and it has been happening ever since I have been forced to work remotely from home (off UK's campus). I want to make it clear the SetFuncBps worked for me (albeit unreliably) while I was on campus. 

Because of this, please pay attention to potential issues that could arise below.

* Since I have been unable to get the SetFuncBps to fully work (cant get the actual gdb session to get hooked in properly), I have been unable to test how well my changes to the SetFuncBps script, and the creation of this python program, will work out. This means there are most certainly bugs between these scripts/.gdb source files that are unintended.
* The python program changes all of the local variables/arguments to a value of 1. This will need to be changed/modified. Potential avenues could be to load in another file with values to change all the variables to, I leave this to future developers to explore.
* Some variables may not be able to be changed to an integer value, which may cause error- needs to be explored more.
* syntax may be off with the `changeVars.gdb` commands, could not be tested.
* Some variables are lists of variable values at breakpoints, the program does not account for this. Example of what I am referring to is line 22782, variable "attrs" of the `gdb.txt` reference file.

## Built With

* [Python3](https://www.python.org/downloads/release/python-360/) -The version of python used.

## Author

* **Alex Schuster**  - [apschuster](https://github.com/apschuster)


