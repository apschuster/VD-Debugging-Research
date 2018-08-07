test.bin: test.elf
	arm-none-eabi-objcopy -O binary test.elf test.bin

test.elf: startup.o test.o
	arm-none-eabi-ld -T test.ld test.o startup.o -o test.elf

test.o: test.c
	arm-none-eabi-gcc -c -mcpu=arm926ej-s test.c -o test.o

startup.o: startup.s
	arm-none-eabi-as -mcpu=arm926ej-s startup.s -o startup.o

clean:
	rm -rf *.o
	rm -rf test.elf

run: test.bin
	qemu-arm -m 128M -M versatilepb -m 128M -kernel $< -serial stdio
