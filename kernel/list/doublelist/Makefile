obj-m := doublelist.o

CUR_PATH := $(shell pwd)
KERNEL_PATH := /usr/src/linux-headers-$(shell uname -r)

all:
	make -C $(KERNEL_PATH) M=$(CUR_PATH) modules
clean:
	make -C $(KERNEL_PATH) M=$(CUR_PATH) clean
