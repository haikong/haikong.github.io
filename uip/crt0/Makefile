#/bin/sh
CROSS_COMPILE = arm-linux-
AS		= $(CROSS_COMPILE)as
LD		= $(CROSS_COMPILE)ld
CC		= $(CROSS_COMPILE)gcc
CPP		= $(CC) -E
AR		= $(CROSS_COMPILE)ar
NM		= $(CROSS_COMPILE)nm
SIZE	= $(CROSS_COMPILE)size

CFLAGS := -Wall -Werror -O2 -g

all:
	$(CC) $(CFLAGS) -c -o crt0.o crt0.s
clean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.bak")
	