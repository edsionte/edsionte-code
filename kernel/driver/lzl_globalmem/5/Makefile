# Filename: Makefile
# Version: 0.2
# Date: 2009.8.19
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

.PHONY : all clean test install remove
all:
	make -C $(KDIR) M=$(PWD) modules
	gcc test/test.c -o test/test
clean:
	make -C $(KDIR) M=$(PWD) clean

test: test/test.c
#	@if [ -f test/test ] ; then \
		rm -f test/test ;\
	fi 
	@./test/test /dev/$(TARGET)
install:
	@./$(TARGET).sh
remove:
	@./clean.sh
	@rm -f test/test
endif
