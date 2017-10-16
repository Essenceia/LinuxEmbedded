# Test Lab - Notes

## Command Line / Environment Variables / User Permissions

1. The logged in users directory
2. Set local variable VV to 3
	- Export local variable as environment variable
	- Enter the bash shell
	- Unest the local variable
	- Exit the bash shell
	- Display the environment variable

3. One can create an alias using echo and redirecting it to the shell configuration file
	- echo alias 'ls="/home/user/ls"' >> .zshrc

4. 3 different streams. Use 2> to redirect the content on stderr. (-p writes the performance report to stdderr)
	- flex -p file.txt 2> resu.txt 

5. chown --reference=result.txt file.txt
'''
bash NUM=$(wc -l </etc/passwd
x)&&NUM=$(expr $NUM / 2 )&& head -n $NUM /etc/passwd
 | tail -1'''

## The commands grep/sed and regular expressions

1.grep '^[^:]*:[^:]*:[0-9]*[02468]:.' /etc/passwd | cut -d: -f1

2.sudo ifconfig wlan0 | grep 'inet '| awk '{print $2}'

## Shell script
'''bash
#!/bin/bash

#init i
i=1
#read line by line
for line in $(cat /etc/passwd)
do
    i=$((i+1))#increment i
    #parse and modify output stream with sed
    echo $i":"$line | sed  's/^\([^:]*\):\([^:]*\):\([^:]*\):\([0-9]*\):\([0-9]*\)::\([^:]*\).*/\1) Username: \2 ,Password: encrypted, UID: \4, GID: \5, Home: \6/'
done
'''

## GNU Make
'''bash
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
	'''
	
All scripts were added in the script folder, I will wright the explanations for eatch of them on the latex.