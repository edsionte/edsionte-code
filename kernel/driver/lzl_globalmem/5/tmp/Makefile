# Filename: Makefile
# Version: 0.1
# Date: 2009.8.11
# Copyleft (C) Zeliang Li <lizeliang.linux@gmail.com>
#
TARGET= globalmem

ifneq ($(KERNELRELEASE),)
 obj-m := $(TARGET).o
 $(TARGET)-objs := module.o global_fops.o
else
 KERNEL := $(shell uname -r)
 KDIR ?= /lib/modules/$(KERNEL)/build
 PWD := $(shell pwd)

all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean
install:
	@sudo ./$(TARGET).sh
remove:
	@sudo ./clean.sh
endif
