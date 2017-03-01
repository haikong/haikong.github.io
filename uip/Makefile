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

CFLAGS := -Wall -Werror  -fpic -nostdlib -g
CFLAGS += -I $(shell pwd)/include

LDFLAGS := -Ttartget.lds -nostdlib 

export CFLAGS LDFLAGS

TOPDIR := $(shell pwd)
export TOPDIR

TARGET := uip

obj-y := swi/
obj-y += sys/
obj-y += init/
obj-y += uart/
obj-y += mmu/
obj-y += dma/
obj-y += lib/
obj-y += lcd/
obj-y += ts/
obj-y += timer/
obj-y += glob/
obj-y += int/
obj-y += dm9000/
obj-y += test/
obj-y += net/
obj-y += miscellaneous/
obj-y += bsp/
obj-y += platform/
obj-y += uip/
obj-y += main/

all :
	make -C crt0
	make -C nand
	make -C ./ -f $(TOPDIR)/Makefile.build
	@#$(CC) $(LDFLAGS) -o $(TARGET) built-in.o
	$(LD) $(LDFLAGS) -o $(TARGET)_elf built-in.o
	$(SIZE) -A -x $(TARGET)_elf > $(TARGET)_elf.txt
	$(OBJCOPY)  -Obinary  $(TARGET)_elf -S $(TARGET).bin
	$(OBJDUMP) -D $(TARGET)_elf > $(TARGET).dis

.PHONY:clean distclean

clean:
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.bak")
	rm -f $(TARGET)_elf $(TARGET).dis $(TARGET).bin $(TARGET)_elf.txt

distclean:
	rm -f $(shell find -name "*.bak")
	rm -f $(shell find -name "*.o")
	rm -f $(shell find -name "*.d")
	rm -f $(TARGET)_elf $(TARGET).dis $(TARGET).bin $(TARGET)_elf.txt