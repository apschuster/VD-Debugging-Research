test.bin: test.elf
	arm-none-eabi-objcopy -O binary test.elf test.bin

test.elf: startup.o stack.o test.o
	arm-none-eabi-ld -T test.ld test.o stack.o startup.o -o test.elf ~/research/newlib-3.0.0/arm-none-eabi/newlib/libc/stdlib/lib.a /usr/bin/../lib/gcc/arm-none-eabi/7.3.1/libgcc.a ~/research/newlib-3.0.0/arm-none-eabi/newlib/libc.a

test.o: stack.h test.c
	arm-none-eabi-gcc -Wall -g -c -mcpu=arm926ej-s test.c -o test.o

stack.o: stack.h stack.c
	arm-none-eabi-gcc -Wall -g -c -mcpu=arm926ej-s stack.c -o stack.o

startup.o: startup.s
	arm-none-eabi-as -mcpu=arm926ej-s startup.s -o startup.o

clean:
	rm -rf *.o
	rm -rf test.elf
	rm -rf test.bin

run: test.bin
	qemu-arm -m 128M -M versatilepb -m 128M -kernel $< -serial stdio

debug: test.bin
	qemu-arm -m 128M -M versatilepb -m 128M -kernel $< -serial stdio -s -S
