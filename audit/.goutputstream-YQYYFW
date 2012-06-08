obj-m :=audit_mod.o
CURRENT_PATH := $(shell pwd)
LINUX_KERNEL := $(shell uname -r)
LINUX_KERNEL_PATH := /lib/modules/$(LINUX_KERNEL)/build/

all:
	make -C $(LINUX_KERNEL_PATH) M=$(CURRENT_PATH) modules 
clean:
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions Module.symvers .Makefile.swp modules.order
	 
