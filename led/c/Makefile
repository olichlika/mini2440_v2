all:
	arm-linux-gcc -c -o led.o led.c
	arm-linux-gcc -c -o start.o start.S
	arm-linux-ld -Ttext 0x00000000 start.o led.o -o led_elf
	arm-linux-objcopy -O binary -S led_elf led.bin
clean:
	rm -f led_on.bin led_on_elf *.o