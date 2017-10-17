TMPGCC=gcc
#if we wanted to cross compile to rpi we would use the toolchain we have downloaded for the kernel compilation : arm-linux-gnueabi-gcc, but to use it we would have to specify a path. In our case we have exported the path to the folder in the vaiable CROSS_COMPILE
ifeq (${CROSS_COMPILE},'')
echo "CROSS_COMPILE is not set";
GCC=${TMPGCC}
else
#to avoid recursice refrencing to GCC variable
GCC=${CROSS_COMPILE}${TMPGCC}
endif
ADD=minmax
DEPS=minmax${HEADER_SUFFIX}
all: main

main: main.o minmax.o
	${GCC} -o main minmax.o main.o

main.o: main.c minmax.h
	${GCC} -c main.c

minmax.o: minmax.c minmax.h
	${GCC} -c minmax.c
clean:
	rm -f *.o main
