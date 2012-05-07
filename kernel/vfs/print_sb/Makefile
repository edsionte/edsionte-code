obj-m := sb.o

CURRENT_PATH:=$(shell pwd)
VERSION_NUM:=$(shell uname -r)
LINUX_KERNEL_PATH:=/usr/src/linux-headers-$(VERSION_NUM)

all:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules
clean:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) clean
