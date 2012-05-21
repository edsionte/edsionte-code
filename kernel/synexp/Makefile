# set to your kernel tree
KERNEL  = /home/linux

# get the Linux architecture. Needed to find proper include file for CFLAGS
ARCH=$(shell uname -m | sed -e s/i.86/i386/ -e s/sun4u/sparc64/ -e s/arm.*/arm/ -e s/sa110/arm/)
# set default flags to compile module
CFLAGS = -D__KERNEL__ -DMODULE -I$(KERNEL)/include
CFLAGS+= -Wall -Wstrict-prototypes -O2 -fomit-frame-pointer -fno-strict-aliasing

all:	sharelist.o 

# get configuration of kernel
include $(KERNEL)/.config
# modify CFLAGS with architecture specific flags
include $(KERNEL)/arch/${ARCH}/Makefile

# enable the module versions, if configured in kernel source tree
ifdef CONFIG_MODVERSIONS
CFLAGS+= -DMODVERSIONS -include $(KERNEL)/include/linux/modversions.h
endif
# enable SMP, if configured in kernel source tree
ifdef CONFIG_SMP
CFLAGS+= -D__SMP__
endif

# note: we are compiling the driver object file and then linking
# we link it into the module. With just one object file as in
# this example this is not needed. We can just load the object
# file produced by gcc 
# link the thread driver module
sharelist.o:     sharelist.c
	gcc $(CFLAGS) -c sharelist.c
clean:
	rm -f *.o 

