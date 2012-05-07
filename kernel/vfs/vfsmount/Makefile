obj-m := sb.o

KERNEL_PATH:=/usr/src/linux-headers-$(shell uname -r)

all:
	make -C $(KERNEL_PATH) M=$(shell pwd) modules
clean:
	make -C $(KERNEL_PATH) M=$(shell pwd) clean
