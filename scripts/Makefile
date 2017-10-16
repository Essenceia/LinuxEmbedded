GCC=gcc
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
