objs = a.o

dep_files := $(patsubst %, .%.d, $(objs))

dep_files := $(wildcard $(dep_files))

CFLAGS = -m32 -Iinclude

test : $(objs)
	gcc $(CFLAGS) -o test $^

ifneq ($(dep_files),)
include $(dep_files)
endif

%.o : %.c
	gcc $(CFLAGS) -c -o $@ $< -MD -MF .$@.d

clean:
	rm *.o test $(dep_files)

distclean:
	rm $(dep_files)

.PHONY: clean 