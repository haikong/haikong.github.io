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

export AS LD CC CPP AR NM
export STRIP OBJCOPY OBJDUMP

CFLAGS := -Wall -Werror -O2 -g
CFLAGS += -I $(shell pwd)/include

LDFLAGS := -Tswi.lds 

export CFLAGS LDFLAGS

TOPDIR := $(shell pwd)
export TOPDIR

TARGET := swi_ta

obj-y := swi/
obj-y += init/
obj-y += sys/
obj-y += int/
obj-y += uart/
obj-y += main/

all :
	make -C ./ -f $(TOPDIR)/Makefile.build
	#$(CC) $(LDFLAGS) -o $(TARGET) built-in.o
	$(LD) $(LDFLAGS) -o $(TARGET)_elf built-in.o
	$(SIZE) -A -x $(TARGET)_elf > $(TARGET)_elf.txt
	$(OBJCOPY) -S -Obinary  $(TARGET)_elf  $(TARGET).bin 
	$(OBJDUMP) -D $(TARGET)_elf > $(TARGET).dis
	
.PHONY:clean distclean

clean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.bak")
	rm -f $(TARGET)_elf $(TARGET).dis $(TARGET).bin

distclean:
	rm -f $(shell find -name "*.bak")
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(TARGET)_elf $(TARGET).dis $(TARGET).bin
