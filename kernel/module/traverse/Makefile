obj-m+=travel_from_init.o

KERNEL_VERSION:=$(shell uname -r)
CUR_PATH:=$(shell pwd)

LINUX_KERNEL_PATH:=/usr/src/linux-headers-$(KERNEL_VERSION)

all:
	make -C $(LINUX_KERNEL_PATH) M=$(CUR_PATH) modules

clean:
	make -C $(LINUX_KERNEL_PATH) M=$(CUR_PATH) clean
