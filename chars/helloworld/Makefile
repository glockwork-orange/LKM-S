# Makefile for Hello World Character Driver

# Name of the module (without .ko)
obj-m += hello_world.o

# Kernel build directory (points to running kernel headers)
KDIR := /lib/modules/$(shell uname -r)/build

# Current directory
PWD := $(shell pwd)

# Default target: build the module
all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

# Clean up build artifacts
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
