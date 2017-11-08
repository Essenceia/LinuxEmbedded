#superlazy vaiable setting
ARCH=arm
CROSS_COMPILE=/home/pookie/Documents/ING4/LinuxEmebeded/kernel_labs/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-
DIR_TO_KERNEL=/home/pookie/Documents/ING4/LinuxEmebeded/kernel_labs/again_2/linux
ccflags-y := -std=gnu99
obj-m += gpio_blink.o
all:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(DIR_TO_KERNEL) M=$(PWD) modules

clean:
	make -C $(DIR_TO_KERNEL) M=$(PWD) clean
