obj-m:=mycdev.o
PWD:=$(shell pwd)
KERNEL_PATH:=/lib/modules/$(shell uname -r)/build

all:
	make -C $(KERNEL_PATH) M=$(PWD) modules
clean:
	make -C $(KERNEL_PATH) M=$(PWD) clean
