obj-m+=myinotify.o

KERNEL_VERSION := $(shell uname -r)
CUR_PATH := $(shell pwd)
KERNEL_PATH := /usr/src/linux-headers-$(KERNEL_VERSION)

all:
	make -C $(KERNEL_PATH) M=$(CUR_PATH) modules
clean:
	make -C $(KERNEL_PATH) M=$(CUR_PATH) clean
