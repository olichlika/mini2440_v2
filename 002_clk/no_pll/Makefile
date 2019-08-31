objs = start.o led.o

dep_files := $(patsubst %, .%.d, $(objs))

dep_files := $(wildcard $(dep_files))

CFLAGS = -Iinclude

led.bin : $(objs)
	arm-linux-ld -Ttext 0x00000000 $^ -o led_elf
	arm-linux-objcopy -O binary -S led_elf $@

ifneq ($(dep_files),)
include $(dep_files)
endif

%.o : %.c
	arm-linux-gcc $(CFLAGS) -c -o $@ $< -MD -MF .$@.d

%.o : %.S
	arm-linux-gcc -c -o $@ $^

clean:
	rm *.o led_elf led.bin $(dep_files)

distclean:
	rm $(dep_files)

.PHONY: clean 