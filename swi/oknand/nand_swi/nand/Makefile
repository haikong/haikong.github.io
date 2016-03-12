#/bin/sh
CROSS_COMPILE = arm-linux-
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
SIZE	= $(CROSS_COMPILE)size

STRIP		= $(CROSS_COMPILE)strip
OBJCOPY		= $(CROSS_COMPILE)objcopy
OBJDUMP		= $(CROSS_COMPILE)objdump

CFLAGS := -Wall -Werror -O1 -fpic -I../include
cur_objs :=$(patsubst %.c,%.o,$(wildcard *.c))



all: $(cur_objs) 
	$(LD) -r -o built-in.o $^

sinclude $(cur_objs:.o=.d)
	
%.o : %.c
	$(CC) $(CFLAGS)  -c -o $@ $<
%.o : %.s
	$(CC) $(CFLAGS)  -c -o $@ $<	
	
%.d:%.c
	@$(CC) $(CFLAGS) -MM $< > $@.$$$$;\
	sed 's,\($*\)\.o[:]*,\1.o $@ :,g' < $@.$$$$ > $@;\
	rm -f $@.$$$$	
clean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.bak")	
