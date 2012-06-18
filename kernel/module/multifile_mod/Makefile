obj-m:=mycdev.o
mycdev-objs := mod.o ops.o

PWD:=$(shell pwd)
KERNEL_PATH:=/lib/modules/$(shell uname -r)/build

all:
	make -C $(KERNEL_PATH) M=$(PWD) modules
clean:
	make -C $(KERNEL_PATH) M=$(PWD) clean
