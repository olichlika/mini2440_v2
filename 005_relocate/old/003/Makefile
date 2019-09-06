objs = start.o main.o uart.o

dep_files := $(patsubst %, .%.d, $(objs))

dep_files := $(wildcard $(dep_files))

CFLAGS = -Iinclude

main.bin : $(objs)
	arm-linux-ld -T sdram.lds $^ -o main.elf
	arm-linux-objcopy -O binary -S main.elf $@
	arm-linux-objdump -D main.elf > main.dis

ifneq ($(dep_files),)
include $(dep_files)
endif

%.o : %.c
	arm-linux-gcc $(CFLAGS) -c -o $@ $< -MD -MF .$@.d

%.o : %.S
	arm-linux-gcc -c -o $@ $^

clean:
	rm -rf *.o *.elf *.bin $(dep_files) *.dis

distclean:
	rm $(dep_files)

.PHONY: clean 