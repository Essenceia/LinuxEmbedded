obj-m += hello.o
all:
	make ARCH=$(ARM) CROSS_COMPILE=$(CROSS_COMPILE) -C $(DIR_TO_KERNEL) M=$(INCLUDE_MOD_PATH) modules

clean:
	make -C $(DIR_TO_KERNEL) M=$(INCLUDE_MOD_PATH) clean
