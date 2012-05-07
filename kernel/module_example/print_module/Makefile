obj-m := print_module.o

CURPATH := $(shell pwd)
KERNEL_PATH := /lib/modules/$(shell uname -r)/build

all:
	make -C $(KERNEL_PATH) M=$(CURPATH) modules
clean:
	make -C $(KERNEL_PATH) M=$(CURPATH) clean
