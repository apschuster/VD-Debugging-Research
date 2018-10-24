Project Github:
	https://github.com/darin-ellis7/VD-Debugging-Research

.vdi system image of the development environment:
	https://drive.google.com/open?id=1TKfPdrQttpnTBNZeRe-HJfjjKYevir8J

These drivers are designed to run on the ARM distribution of QEMU. The version of QEMU used in designing and testing these drivers (qemu-3.0.0) was installed via the following commands:

	wget http://download.qemu.org/qemu-3.0.0-rc0.tar.xz 
		(Note: in the past the qemu website has been a moving target, so this url may not hold in the future)
	tar xvJf qemu-3.0.0-rc0.tar.xz
	cd qemu-3.0.0-rc0
	sudo apt update 
		(Note: all of the following until the configure command are to resolve dependencies)
	sudo apt upgrade
	sudo apt apt install build-essential 
		(Note: this may render some of the following commands unnecessary, but they are included for the sake of completeness)
	sudo apt-get install python
	sudo apt-get install pkg-config
	sudo apt-get install zlib1g-dev
	sudo apt-get install libgtk-3-dev
	./configure --enable-debug --enable-gtk --extracflags=-O0
	make

You will want to alias the location of the ARM QEMU executable to a variable. In the development this was done via the following command:

	alias qemu-arm=”/home/kyle/research/qemu-3.0.0-rc0/arm-softmmu/qemu-system-arm”

Now we require the arm-none-eabi toolchain to compile the drivers. This can be done via the following commands:

	sudo add-apt-repository ppa:team-gcc-arm-embedded/ppa
	sudo apt-get update
	sudo apt-get upgrade
	sudo apt-get install gcc-arm-embedded

Finally some of the files require implimentations of library functions from newlib. The version of newlib used used for these drivers (newlib-3.0.0) can be downloaded and installed via the following commands:

	wget ftp://sources.redhat.com/pub/newlib/newlib-3.0.0.tar.gz
	tar xzf newlib-3.0.0.tar.gz
	cd newlib-3.0.0
	./configure --target arm-none-eabi --disable-newlib-supplied-syscalls --enable debug
	make

Note that the locations of the various libraries is dependent on where newlib is installed. As such, the Makefiles for the various drivers may need revision to account for the new locations.

You should now be able to compile and run the drivers. Most drivers have a 'make run' command included that will run the command necessary to run the driver, however make does not accomidate aliases, so the command will have to be copied into the command line manually.

If you wish to debug the program you may either invoke the 'make debug' command included with most of the drivers, or input the following command:

	qemu-arm -m 128M -M versatilepb -m 128M -kernel test.bin -serial stdio -s -S

Note that to run the drivers normally, simply ommit -s and -S flags. Next open another instance of the terminal in the same directory. Run the ARM version of GDB via the following:

	arm-none-eabi-gdb

Then input the following commands to gdb in order to hook into the Virtual Machine and read the appropriate symbol-table.

	target remote localhost:1234
	symbol-file test.elf

