TARGET= globalmem

ifneq ($(KERNELRELEASE),)
 obj-m := $(TARGET).o
 $(TARGET)-objs := module.o global_fops.o
else
 KERNEL := $(shell uname -r)
 KDIR ?= /lib/modules/$(KERNEL)/build
 PWD := $(shell pwd)

.PHONY : all clean install remove
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	make -C $(KDIR) M=$(PWD) clean

test: test/test.c
#	@if [ -f test/test ] ; then \
		rm -f test/test ;\
	fi 
	gcc test/test.c -o test/test
	@./test/test /dev/$(TARGET)
install:
	@./$(TARGET).sh
remove:
	@./clean.sh
	@rm -f test/test
endif
